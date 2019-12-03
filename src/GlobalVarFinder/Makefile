CXX := /usr/bin/clang
LLVM_SRC_PATH := /usr/lib/llvm-6.0
LLVM_BUILD_PATH := /usr/lib/llvm-6.0/build
LLVM_BIN_PATH := /usr/lib/llvm-6.0/bin

LIB = -fno-rtti \
    -lclangFrontend -lclangDriver -lclangTooling \
    -lclangDriver -lclangTooling -lclangFrontendTool \
    -lclangFrontend -lclangDriver -lclangSerialization \
    -lclangCodeGen -lclangParse -lclangSema \
    -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers \
    -lclangStaticAnalyzerCore -lclangAnalysis \
    -lclangARCMigrate -lclangRewriteFrontend \
    -lclangEdit -lclangAST \
    -lclangLex -lclangBasic  -lclangCodeGen -lclangSema \
    -lclangAnalysis  -lclangAST -lclangParse -lclangLex \
    -lclangASTMatchers \
    -lclangBasic -lclang

gm: TREEVIEW.cpp
	clang++ `llvm-config --cxxflags --ldflags` TREEVIEW.cpp $(LIB) `llvm-config --libs --system-libs` -o gm

PROGRAMS = TREEVIEW
clean:
	@rm -f TREEVIEW.o gm
