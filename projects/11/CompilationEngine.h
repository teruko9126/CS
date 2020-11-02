#ifndef _COMPILATIONENGINE_
#define _COMPILATIONENGINE_

#include <iostream>
#include <string>
#include <fstream>
#include "JackTokenizer.h"
using namespace std;

class CompilationEngine
{
public:
  CompilationEngine(JackTokenizer *tokenizer,string filename);
  ~CompilationEngine(void);
  void compileClass(void);
  void compileClassVarDec(void);
  void compileSubroutine(void);
  void compileSubroutineBody(void);
  void compileParameterList(void);
  void compileVarDec(void);
  void compileStatements(void);
  void compileDo(void);
  void compileLet(void);
  void compileWhile(void);
  void compileReturn(void);
  void compileIf(void);
  void compileExpression(void);
  void compileTerm(void);
  void compileExpressionList(void);

  void no_compileType(void);
  void no_compileClassName(void);
  void no_compileSubroutineName(void);
  void no_compileVarName(void);
  void no_compileStatement(void);
  void no_compileSubroutineCall(void);
  void no_compileOp(void);
  void no_compileUnaryOp(void);
  void no_KeywordConstant(void);

  void outkeyword();
  void outsymbol();
  void outinteger();
  void outstring();
  void outidentifier();

  void getempty(void);

private:
  ofstream outf;
  JackTokenizer *current_tokenizer;
  string current_filename;
};

#endif
