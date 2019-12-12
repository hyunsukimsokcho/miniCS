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

// V[idx] = val, with resizing if necessary
void add_to_vec(std::vector<int> &V, size_t idx, int val){
  while(V.size() <= idx) V.push_back(0);
  V[idx] = val;
}

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

// true iff the i-th line references a global variable
std::vector<int> has_global;

// find the global variable references and print them
struct GlobalVarPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    Expr const *gref = result.Nodes.getNodeAs<Expr>("globalRef");
    VarDecl const *gvar = result.Nodes.getNodeAs<VarDecl>("globalVar");
    if(fd && gref && gvar){
      int line = source_lineno(gref->getSourceRange(), &manager, false);
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " refers to global " << gvar->getNameAsString() << " at ";
      std::cout << source_location(gref->getSourceRange(), &manager, false);
      std::cout << std::endl;
      add_to_vec(has_global, line, 1);
    }
  }
};

/* ---------------------------------------------------------------- */
/*
// global variable unary operation matcher
static StatementMatcher global_unary_match =
  declRefExpr(
    to(
      varDecl(
        hasGlobalStorage()
      ).bind("globalVar")
    ), 
    hasAncestor(
      unaryOperator(
        hasAncestor(
          functionDecl().bind("function")
        )
      ).bind("operator")
    )
  ).bind("globalRef");

// true iff the i-th line writes to a global variable
std::vector<int> has_global_write;

// find the global variable references and print them
struct GlobalUnaryPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    Expr const *gref = result.Nodes.getNodeAs<Expr>("globalRef");
    VarDecl const *gvar = result.Nodes.getNodeAs<VarDecl>("globalVar");
    UnaryOperator const *uop = result.Nodes.getNodeAs<UnaryOperator>("operator");
    if(fd && gref && gvar && uop){
      if(!uop->isIncrementOp() && !uop->isDecrementOp()) return;
      int line = source_lineno(gref->getSourceRange(), &manager, false);
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " WRITES to global " << gvar->getNameAsString() << " at ";
      std::cout << source_location(gref->getSourceRange(), &manager, false);
      std::cout << std::endl;
      add_to_vec(has_global_write, line, 1);
    }
  }
};
*/
/* ---------------------------------------------------------------- */
/*
// global variable assignment matcher (either LHS or RHS)
static StatementMatcher assignment_match =
  declRefExpr(
    to(
      varDecl(
        hasGlobalStorage()
      ).bind("globalVar")
    ), 
    hasAncestor(
      binaryOperator(
        hasAncestor(
          functionDecl().bind("function")
        )
      ).bind("operator")
    )
  ).bind("globalRef");

// find the global variable assignments and print them
struct GlobalAssignPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    Expr const *gref = result.Nodes.getNodeAs<Expr>("globalRef");
    VarDecl const *gvar = result.Nodes.getNodeAs<VarDecl>("globalVar");
    BinaryOperator const *bop = result.Nodes.getNodeAs<BinaryOperator>("operator");
    if(fd && gref && gvar && bop){
      if(bop->getLHS() != gvar) return;
      //if(!uop->isIncrementOp() && !uop->isDecrementOp()) return;
      int line = source_lineno(gref->getSourceRange(), &manager, false);
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " WRITES to global " << gvar->getNameAsString() << " at ";
      std::cout << source_location(gref->getSourceRange(), &manager, false);
      std::cout << std::endl;
      add_to_vec(has_global_write, line, 1);
    }
  }
};
*/
/* ---------------------------------------------------------------- */

// scope matcher
static StatementMatcher scope_match =
  compoundStmt(
    hasAncestor(
      functionDecl().bind("function")
    )
  ).bind("scope");

// +1 if a scope starts at the i-th line, -1 if ends, 0 otherwise
std::vector<int> scope_delta;

// find the scopes and print them
struct ScopePrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    CompoundStmt const *cs = result.Nodes.getNodeAs<CompoundStmt>("scope");
    if(fd && cs && fd->getNameAsString() != "main"){
      int line_begin = source_lineno(cs->getSourceRange(), &manager, false);
      int line_end = source_lineno(cs->getSourceRange(), &manager, true);
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " contains a scope ";
      std::cout << source_location(cs->getSourceRange(), &manager, true);
      std::cout << std::endl;
      add_to_vec(scope_delta, line_begin, 1);
      add_to_vec(scope_delta, line_end, -1);
    }
  }
};

/* ---------------------------------------------------------------- */

// return matcher
static StatementMatcher return_match =
  returnStmt().bind("return");

// check if there is a return statement in line i
std::vector<int> has_return;

// find the return statements and print them
struct ReturnPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    ReturnStmt const *rs = result.Nodes.getNodeAs<ReturnStmt>("return");
    if(rs){
      int line = source_lineno(rs->getSourceRange(), &manager, false);
      std::cout << "Return statement in line " << line << std::endl;
      add_to_vec(has_return, line, 1);
    }
  }
};

/* ---------------------------------------------------------------- */

int main(int argc, const char *argv[]){
  CommonOptionsParser optionsParser(argc, argv, toolCategory);
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());

  GlobalVarPrinter gvprinter;
  //GlobalUnaryPrinter guprinter;
  //GlobalAssignPrinter gaprinter;
  ScopePrinter sprinter;
  ReturnPrinter rprinter;
  MatchFinder MF;

  MF.addMatcher(global_match, &gvprinter);
  //MF.addMatcher(global_unary_match, &guprinter);
  MF.addMatcher(scope_match, &sprinter);
  MF.addMatcher(return_match, &rprinter);
  //MF.addMatcher(assignment_match, &gaprinter);
  tool.run(newFrontendActionFactory(&MF).get());

  for(size_t l = 0; l < scope_delta.size(); l++){
    if(l+1 < scope_delta.size() && scope_delta[l+1]) continue;
    int depth = 0;
    for(size_t i = 0; i <= l; i++) depth+= scope_delta[i];
    if(depth == 0) continue;
    int start_depth = depth;
    bool met_global = false;
    for(size_t r = l+1; r < scope_delta.size(); r++){
      if(r < has_return.size() && has_return[r]) break;
      if(r < has_global.size() && has_global[r]) met_global = true;
      depth+= scope_delta[r];
      if(depth == 0) break;
      if(met_global && depth == start_depth) std::cout << l << ' ' << r << std::endl;
    }
  }
}
