#include <llvm/Support/CommandLine.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

static llvm::cl::OptionCategory toolCategory("gm options");
static llvm::cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);

// global variable reference matcher
static StatementMatcher global_match =
  declRefExpr(
    to(
      varDecl(
        hasGlobalStorage()
      ).bind("globalVar")
    ), hasAncestor(
      functionDecl().bind("function")
    )
  ).bind("globalRef");

// scope matcher
static StatementMatcher scope_match =
  compoundStmt(
    hasAncestor(
      functionDecl().bind("function")
    )
  ).bind("scope");

/* ------------------------------------------------ */

// get the location of the source range
std::string source_location(SourceRange r,
    SourceManager const *sm, bool use_end)
{
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

// find the global variable references and print them
struct GlobalVarPrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    Expr const *gref = result.Nodes.getNodeAs<Expr>("globalRef");
    VarDecl const *gvar = result.Nodes.getNodeAs<VarDecl>("globalVar");
    if(fd && gref && gvar){
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " refers to global " << gvar->getNameAsString() << " at ";
      std::cout << source_location(gref->getSourceRange(), &manager, false);
      std::cout << std::endl;
    }
  }
};

// find the scopes and print them
struct ScopePrinter: public clang::ast_matchers::MatchFinder::MatchCallback{
  virtual void run(MatchFinder::MatchResult const &result) override{
    SourceManager &manager(
      const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    FunctionDecl const *fd = result.Nodes.getNodeAs<FunctionDecl>("function");
    CompoundStmt const *cs = result.Nodes.getNodeAs<CompoundStmt>("scope");
    if(fd && cs){
      std::cout << "Function " << fd->getNameAsString();
      std::cout << " contains a scope ";
      std::cout << source_location(cs->getSourceRange(), &manager, true);
      std::cout << std::endl;
    }
  }
};

/* ------------------------------------------------ */

int main(int argc, const char *argv[]){
  CommonOptionsParser optionsParser(argc, argv, toolCategory);
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());

  GlobalVarPrinter gvprinter;
  ScopePrinter sprinter;
  MatchFinder MF;
  MF.addMatcher(global_match, &gvprinter);
  MF.addMatcher(scope_match, &sprinter);
  tool.run(newFrontendActionFactory(&MF).get());
}
