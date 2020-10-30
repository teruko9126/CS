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
  CompilationEngine(string filename);
  ~CompilationEngine(void);
  void compileClass(void);
  void compileClassVarDec(void);
  void compileSubroutine(void);
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

private:
  ofstream outf;
  JackTokenizer tokenizer1;
};

#endif
