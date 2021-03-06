#ifndef _COMPILATIONENGINE_
#define _COMPILATIONENGINE_

#include <iostream>
#include <string>
#include <fstream>
#include "JackTokenizer.h"
#include "VMWriter.h"
#include "SymbolTable.h"

using namespace std;

namespace CEhelper {
  string keyword2string(KeyWord Key) {
    switch (Key) {
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

  symboltable::Kind keyword2kind(KeyWord Key) {
    switch (Key) {
      case STATIC:
        return symboltable::STATIC;
      case FIELD:
        return symboltable::FIELD;
      case VAR:
        return symboltable::VAR;
    }
  }

  vmwriter::Segment kind2Segment(symboltable::Kind kind) {
    switch (kind) {
      case symboltable::STATIC:
        return vmwriter::STATIC;
      case symboltable::FIELD:
        return vmwriter::THIS;
      case symboltable::ARG:
        return vmwriter::ARG;
      case symboltable::VAR:
        return vmwriter::LOCAL;
    }
  }
} // namespace CEhelper
class CompilationEngine {
public:
  CompilationEngine(JackTokenizer *tokenizer, Symboltable *symboltable, VMWriter *vmwriter, string filename);

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

  symboltable::Kind keyword2kind(string keyword);

  string keyword2string(KeyWord keyword);

private:
  int loopnumber;
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
  int stringnumber;
  string usestring;
};

#endif
