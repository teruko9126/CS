#ifndef _COMPILATIONENGINE_
#define _COMPILATIONENGINE_

#include <iostream>
#include <string>
#include <fstream>
#include "JackTokenizer.h"
#include "VMWriter.h"
#include "SymbolTable.h"
using namespace std;

namespace CEhelper
{
  string keyword2string(KeyWord Key)
  {
    switch(Key)
    {
      case CLASS:
        return "class";
      case METHOD:
        return "method";
      case FUNCTION:
        return "function";
      case CONSTRUCTOR:
        return "constructor";
      case INT:
        return "int";
      case BOOLEAN:
        return "boolean";
      case CHAR:
        return "char";
      case VOID:
        return "void";
      case VAR:
        return "var";
      case STATIC:
        return "static";
      case FIELD:
        return "field";
      case LET:
        return "let";
      case DO:
        return "do";
      case IF:
        return "if";
      case ELSE:
        return "else";
      case WHILE:
        return "while";
      case RETURN:
        return "return";
      case TRUE:
        return "true";
      case FALSE:
        return "false";
      case WORDNULL:
        return "null";
      case THIS:
        return "this";
      default:
        return "error keyword2string";
    }
  }

  symboltable::Kind keyword2kind(KeyWord Key)
  {
    switch(Key)
    {
      case STATIC:
        return symboltable::STATIC;
      case FIELD:
        return symboltable::FIELD;
      case VAR:
        return symboltable::VAR;
    }
  }
}
class CompilationEngine
{
public:
  CompilationEngine(JackTokenizer *tokenizer,Symboltable *symboltable,VMWriter *vmwriter,string filename);
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

  symboltable::Kind keyword2kind(string keyword);
  string keyword2string(KeyWord keyword);

private:
  ofstream outf;
  JackTokenizer *TOKEN;
  Symboltable *SYM;
  VMWriter *VM;
  string current_filename;
  string classname;
  string current_functionname;
  string subroutinename;
  KeyWord subroutineType;
  int numArgs;
};

#endif
