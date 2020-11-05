#include "CompilationEngine.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

CompilationEngine::CompilationEngine(JackTokenizer *tokenizer, Symboltable *symboltable, VMWriter *vmwriter,
                                     string filename) {
  current_filename = filename;
  TOKEN = tokenizer;
  SYM = symboltable;
  VM = vmwriter;
  compileClass();
}

CompilationEngine::~CompilationEngine() {
  if (outf.is_open()) {
    outf.close();
  }
}

void CompilationEngine::compileClass() {

  TOKEN->advance();

  assert(TOKEN->keyWord() == CLASS);
  TOKEN->advance();

  assert(TOKEN->tokenType() == IDENTIFIER);
  classname = TOKEN->identifier();
  TOKEN->advance();

  assert(TOKEN->symbol() == "{");
  TOKEN->advance();

  while (TOKEN->keyWord() == STATIC || TOKEN->keyWord() == FIELD) {
    compileClassVarDec();
  }
  while (TOKEN->keyWord() == CONSTRUCTOR || TOKEN->keyWord() == FUNCTION ||
         TOKEN->keyWord() == METHOD) {
    compileSubroutine();
  }

  assert(TOKEN->symbol() == "}");
}

void CompilationEngine::compileClassVarDec() {

  string name, type;
  symboltable::Kind kind = CEhelper::keyword2kind(TOKEN->keyWord());
  TOKEN->advance();

  if (TOKEN->tokenType() == KEYWORD) {
    type = CEhelper::keyword2string(TOKEN->keyWord());
  }else if(TOKEN->tokenType() == IDENTIFIER){
    type = TOKEN->identifier();
  }

  TOKEN->advance();
  while (TOKEN->symbol() != ";") {
    switch (TOKEN->tokenType()){
      case SYMBOL:
        assert(TOKEN->symbol() == ",");
        TOKEN->advance();
        break;
      case IDENTIFIER:
        name = TOKEN->identifier();
        SYM->define(name,type,kind);
        TOKEN->advance();
        break;
    }
  }

  assert(TOKEN->symbol() == ";");
  TOKEN->advance();
}

void CompilationEngine::compileSubroutine() {

  SYM->startSubroutine();

  subroutineType = TOKEN->keyWord();
  //if(subroutineType == METHOD)
  //TODOここ以降よくわからん！！！！！！！(181行以降)どうしてthisで置いている？？？
  TOKEN->advance();

  string returntype; //のちに返り値について考えるときに使う
  if(TOKEN->tokenType() == KEYWORD){
    returntype = CEhelper::keyword2string(TOKEN->keyWord());
  }else if(TOKEN->tokenType() == IDENTIFIER){
    returntype = TOKEN->identifier();
  }
  TOKEN->advance();

  assert(TOKEN->tokenType() == IDENTIFIER);
  subroutinename = classname + "." + TOKEN->identifier();
  TOKEN->advance();

  assert(TOKEN->symbol() == "(");
  TOKEN->advance();

  compileParameterList();

  assert(TOKEN->symbol() == ")");
  TOKEN->advance();

  compileSubroutineBody();
}

void CompilationEngine::compileParameterList() {
  string name,type;
  symboltable::Kind kind = symboltable::ARG;
  bool truetype = true;
  while(TOKEN->symbol() != ")") {
    switch (TOKEN->tokenType()) {
      case KEYWORD:
        type = CEhelper::keyword2string(TOKEN->keyWord());
        truetype = false;
        TOKEN->advance();
        break;
      case IDENTIFIER:
        if (truetype) {
          type = TOKEN->identifier();
          truetype = false;
        } else {
          name = TOKEN->identifier();
          SYM->define(name, type, kind);
          truetype = true;
        }
        TOKEN->advance();
        break;
    }
  }
}

void CompilationEngine::compileSubroutineBody() {
  assert(TOKEN->symbol() == "{");
  TOKEN->advance();

  while (TOKEN->keyWord() == VAR)
    compileVarDec();

  int nLocals = SYM->VarCount(symboltable::VAR);

  VM->writeFunction(subroutinename,nLocals);

  //TODO constructorとmethodの中身についてはあとで例を見ながら実装！！

  compileStatements();

  assert(TOKEN->symbol() == "}");
  TOKEN->advance();
}

void CompilationEngine::compileVarDec() {
  string name,type;
  symboltable::Kind kind = symboltable::VAR;

  assert(TOKEN->keyWord() == VAR);
  TOKEN->advance();

  //this define type
  if(TOKEN->tokenType() == KEYWORD){
    type = CEhelper::keyword2string(TOKEN->keyWord());
  }else if(TOKEN->tokenType() == IDENTIFIER){
    type = TOKEN->identifier();
  }
  TOKEN->advance();

  while(TOKEN->getcurrentcommand() != ";") {
    switch (TOKEN->tokenType()) {
      case SYMBOL:
        assert(TOKEN->symbol() == ",");
        TOKEN->advance();
        break;
      case IDENTIFIER:
        name = TOKEN->identifier();
        SYM->define(name, type, kind);
        TOKEN->advance();
        break;
    }
  }
    assert(TOKEN->symbol() == ";");
    TOKEN->advance();
}

void CompilationEngine::compileStatements() {
  while(TOKEN->keyWord() == LET || TOKEN->keyWord() == IF || TOKEN->keyWord() == WHILE || TOKEN->keyWord() == DO || TOKEN->keyWord() == RETURN){
    switch(TOKEN->keyWord()) {
      case LET:
        compileLet();
        break;
      case IF:
        compileIf();
        break;
      case WHILE:
        compileWhile();
        break;
      case DO:
        compileDo();
        break;
      case RETURN:
        compileReturn();
        break;
    }
  }
}

void CompilationEngine::compileLet() {
  outkeyword();
  no_compileVarName();
  if (TOKEN->symbol() == "[") {
    outsymbol();
    compileExpression();
    outsymbol();//"]"
  }
  outsymbol();
  compileExpression();
  outsymbol();
}

void CompilationEngine::compileIf() {
  outkeyword(); // if
  outsymbol(); // (
  compileExpression();
  outsymbol(); // )
  outsymbol(); // {
  compileStatements();
  outsymbol(); // }
  if (TOKEN->keyWord() == ELSE) {
    outkeyword(); // else
    outsymbol(); // {
    compileStatements();
    outsymbol(); // }
  }
}

void CompilationEngine::compileWhile() {
  outkeyword(); //while
  outsymbol(); // (
  compileExpression();
  outsymbol(); // )
  outsymbol(); // {
  compileStatements();
  outsymbol(); // }
}

void CompilationEngine::compileDo() {
  assert(TOKEN->keyWord() == DO);
  TOKEN->advance();

  string name1 = TOKEN->identifier();
  TOKEN->advance();

  if(TOKEN->symbol() == "("){
    TOKEN->advance();

    //TODO　なぜpointerをpushする？？？？？今はまだここ通らないのでスルーする
  }else if(TOKEN->symbol() == "."){
    TOKEN->advance();

    string name2 = TOKEN->identifier();
    TOKEN->advance();
    //TODOなんだこいつ？？？？？？
    if(SYM->kindOf(name1) != symboltable::NONE){

    }else{
      assert(TOKEN->symbol() == "(");
      TOKEN->advance();

      compileExpressionList();
      VM->writeCall(name1 + "." + name2,numArgs);
      numArgs = 0;

      assert(TOKEN->symbol() == ")");
      TOKEN->advance();
    }
  }

  //TODO pop temp は必要か？？

  assert(TOKEN->symbol() == ";");
  TOKEN->advance();
}

void CompilationEngine::compileReturn() {
  assert(TOKEN->keyWord() == RETURN);
  TOKEN->advance();

  if(TOKEN->getcurrentcommand() != ";"){
    compileExpression();
  }

  VM->writeReturn();

  assert(TOKEN->symbol() == ";");
  TOKEN->advance();
}

void CompilationEngine::compileExpressionList() {
  if (TOKEN->getcurrentcommand() != ")") {
    compileExpression();
    numArgs++;

    while (TOKEN->symbol() == ",") {
      TOKEN->advance();

      compileExpression();
      numArgs++;
    }
  }
}

void CompilationEngine::compileExpression() {
  compileTerm();
  while (TOKEN->getcurrentcommand() == "+" || TOKEN->getcurrentcommand() == "-" ||
         TOKEN->getcurrentcommand() == "*" || TOKEN->getcurrentcommand() == "/" ||
         TOKEN->getcurrentcommand() == "&" || TOKEN->getcurrentcommand() == "|" ||
         TOKEN->getcurrentcommand() == "<" || TOKEN->getcurrentcommand() == ">" ||
         TOKEN->getcurrentcommand() == "="){

    string op = TOKEN->symbol();
    TOKEN->advance();

    compileTerm();
    if(op == "+") VM->writeArithmetic(vmwriter::ADD);
    else if(op == "-") VM->writeArithmetic(vmwriter::SUB);
    else if(op == "*") VM->writeCall("Math.multiply",2);
    else if(op == "/") VM->writeCall("Math.divide",2);
    else if(op == "&") VM->writeArithmetic(vmwriter::AND);
    else if(op == "|") VM->writeArithmetic(vmwriter::OR);
    else if(op == "<") VM->writeArithmetic(vmwriter::LT);
    else if(op == ">") VM->writeArithmetic(vmwriter::GT);
    else if(op == "=") VM->writeArithmetic(vmwriter::EQ);
    else if(op == "~") VM->writeArithmetic(vmwriter::NOT);
  }
}

void CompilationEngine::compileTerm() {
  switch (TOKEN->tokenType()) {
    case KEYWORD:
      break;
    case SYMBOL:
      if(TOKEN->symbol() == "("){
        TOKEN->advance();

        compileExpression();

        assert(TOKEN->symbol() == ")");
        TOKEN->advance();
      }
      break;
    case INT_CONST:
      VM->writePush(vmwriter::CONST,TOKEN->intVal());
      TOKEN->advance();
      break;
    case STRING_CONST:
      break;
    case IDENTIFIER:
      break;
  }
}

void CompilationEngine::no_compileType() {
  if (TOKEN->tokenType() == KEYWORD) {
    outkeyword();
  } else if (TOKEN->tokenType() == IDENTIFIER) {
    outidentifier();
  }
}

void CompilationEngine::no_compileClassName() {
  classname = TOKEN->identifier();
  TOKEN->advance();
}

void CompilationEngine::no_compileVarName() {
  outidentifier();
}

void CompilationEngine::no_compileSubroutineName() {
  outidentifier();
}

void CompilationEngine::no_compileStatement() {
  if (TOKEN->keyWord() == LET) {
    compileLet();
  }
  if (TOKEN->keyWord() == IF) {
    compileIf();
  }
  if (TOKEN->keyWord() == WHILE) {
    compileWhile();
  }
  if (TOKEN->keyWord() == DO) {
    compileDo();
  }
  if (TOKEN->keyWord() == RETURN) {
    compileReturn();
  }
}

void CompilationEngine::no_compileOp() {
  outsymbol();
}

void CompilationEngine::no_compileUnaryOp() {
  outsymbol();
}

void CompilationEngine::no_KeywordConstant() {
  outkeyword();
}

void CompilationEngine::no_compileSubroutineCall() {
  no_compileSubroutineName(); //
  //maybe this isn't subroutineName, this is classname or varname
  //but all of this is identifier ,so it's ok

  if (TOKEN->symbol() == ".") {
    outsymbol();
    no_compileSubroutineName();
  }
  outsymbol(); // (
  compileExpressionList();
  outsymbol(); // )
}

void CompilationEngine::outkeyword() {
  outf << "<keyword> " << TOKEN->keyword2command(TOKEN->keyWord()) << " </keyword>" << endl;
  TOKEN->advance();
}

void CompilationEngine::outsymbol() {
  outf << "<symbol> " << TOKEN->symbol() << " </symbol>" << endl;
  TOKEN->advance();
}

void CompilationEngine::outinteger() {
  outf << "<integerConstant> " << TOKEN->intVal() << " </integerConstant>" << endl;
  TOKEN->advance();
}

void CompilationEngine::outstring() {
  outf << "<stringConstant> " << TOKEN->stringVal() << " </stringConstant>" << endl;
  TOKEN->advance();
}

void CompilationEngine::outidentifier() {
  outf << "<identifier> " << TOKEN->identifier() << " </identifier>" << endl;
  TOKEN->advance();
}






