#include <llvm/Support/CommandLine.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

static llvm::cl::OptionCategory toolCategory("gm options");
static llvm::cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);

// get the location of the source range
std::string source_location(SourceRange r,
    SourceManager const *sm, bool use_end){
  if(!sm) return "UNDEFINED";
  SourceLocation loc = r.getBegin();
  SourceLocation loc_end = r.getEnd();
  PresumedLoc ploc = sm->getPresumedLoc(sm->getSpellingLoc(loc));
  std::string line = std::to_string(ploc.getLine());
  std::string col = std::to_string(ploc.getColumn());
  if(use_end){
    PresumedLoc ploc_end = sm->getPresumedLoc(sm->getSpellingLoc(loc_end));
    std::string line_end = std::to_string(ploc_end.getLine());
    std::string col_end = std::to_string(ploc_end.getColumn());
    return line + ":" + col + " --- " + line_end + ":" + col_end;
  }
  return line + ":" + col;
} 

// get the line number of the range
int source_lineno(SourceRange r,
    SourceManager const *sm, bool use_end){
  assert(sm);
  SourceLocation loc;
  if(!use_end) loc = r.getBegin();
  else loc = r.getEnd();
  PresumedLoc ploc = sm->getPresumedLoc(sm->getSpellingLoc(loc));
  return ploc.getLine();
}

bool valid_funcname(std::string fn){
  if(fn.size() >= 2 && fn[0] == '_' && fn[1] == '_')
    return false;
  if(fn.size() >= 3 && fn[1] == '_' && fn[2] == '_')
    return false;
  return fn.size() > 0 && fn != "main";
}

/* ------------------------------------------------ */

// global variable reference matcher
static StatementMatcher global_match =
  declRefExpr(
    to(
      varDecl(
        hasGlobalStorage()
      ).bind("globalVar")
    ),
    hasAncestor(
      functionDecl().bind("function")
    )
  ).bind("globalRef");

// which lines reference a global variable
std::vector<int> has_global;

// find the global variable references and print them
struct GlobalVarPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    Expr const *gref = result.Nodes.getNodeAs<Expr>("globalRef");
    VarDecl const *gvar = result.Nodes.getNodeAs<VarDecl>("globalVar");
    if(fd && gref && gvar && valid_funcname(fd->getNameAsString())){
      int line = source_lineno(gref->getSourceRange(), &manager, false);
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " refers to global " << gvar->getNameAsString() << " at ";
      std::cout << source_location(gref->getSourceRange(), &manager, false);
      std::cout << std::endl;
      has_global.push_back(line);
    }
  }
};

/* ---------------------------------------------------------------- */

// scope matcher
static StatementMatcher scope_match =
  compoundStmt(
    hasAncestor(
      functionDecl().bind("function")
    )
  ).bind("scope");

std::vector<std::pair<int, int>> scopes;

// find the scopes and print them
struct ScopePrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    CompoundStmt const *cs = result.Nodes.getNodeAs<CompoundStmt>("scope");
    if(fd && cs && valid_funcname(fd->getNameAsString())){
      int line_begin = source_lineno(cs->getSourceRange(), &manager, false);
      int line_end = source_lineno(cs->getSourceRange(), &manager, true);
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " contains a scope ";
      std::cout << source_location(cs->getSourceRange(), &manager, true);
      std::cout << std::endl;
      scopes.push_back({line_begin, line_end});
    }
  }
};

/* ---------------------------------------------------------------- */

// return matcher
static StatementMatcher return_match =
  returnStmt(
    hasAncestor(
      functionDecl().bind("function")
    )
  ).bind("return");

// which lines have a return statement
std::vector<int> has_return;

// find the return statements and print them
struct ReturnPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    ReturnStmt const *rs = result.Nodes.getNodeAs<ReturnStmt>("return");
    if(rs && valid_funcname(fd->getNameAsString())){
      int line = source_lineno(rs->getSourceRange(), &manager, false);
      std::cout << "Return statement in line " << line << std::endl;
      has_return.push_back(line);
    }
  }
};

/* ---------------------------------------------------------------- */

bool valid_range(int s, int e){
  bool global_flag = false;
  // global reference
  for(int x: has_global){
    if(s < x && x <= e) global_flag = true;
  }
  if(!global_flag) return false;

  // scope relation
  bool contained_somewhere = false;
  for(auto [sp, ep]: scopes){
    if(sp <= s && e < ep) contained_somewhere = true;
    else if(s < sp && ep <= e) continue; // contains the scope
    else if(ep <= s || e < sp) continue; // disjoint with scope
    else return false;
  }
  if(!contained_somewhere) return false;

  // return
  for(int x: has_return){
    if(s < x && x <= e) return false;
  }
  return true;
}

/* ---------------------------------------------------------------- */

int main(int argc, const char *argv[]){
  CommonOptionsParser optionsParser(argc, argv, toolCategory);
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());

  GlobalVarPrinter gvprinter;
  ScopePrinter sprinter;
  ReturnPrinter rprinter;
  MatchFinder MF;

  MF.addMatcher(global_match, &gvprinter);
  MF.addMatcher(scope_match, &sprinter);
  MF.addMatcher(return_match, &rprinter);
  tool.run(newFrontendActionFactory(&MF).get());

  int endline = -1;
  for(auto [s, e]: scopes) endline = std::max(endline, e);
  for(int s=1; s<=endline; s++) for(int e=s+1; e<=endline; e++){
    if(valid_range(s, e)) std::cout << s << ' ' << e << std::endl;
  }
}
