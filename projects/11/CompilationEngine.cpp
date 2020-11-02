#include "CompilationEngine.h"
#include "JackTokenizer.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

int num = 0;

CompilationEngine::CompilationEngine(JackTokenizer *tokenizer, string filename) {
  current_filename = filename;
  int dot = current_filename.find(".");
  filename = filename.substr(0, dot) + "complete.xml";
  outf.open(filename.c_str());
  current_tokenizer = tokenizer;
  compileClass();
}

CompilationEngine::~CompilationEngine() {
  if (outf.is_open()) {
    outf.close();
  }
}

void CompilationEngine::compileClass() {
  getempty();
  outf << "<class>" << endl;
  num++;
  current_tokenizer->advance();
  outkeyword(); //class
  no_compileClassName();
  outsymbol();
  while (current_tokenizer->keyWord() == STATIC || current_tokenizer->keyWord() == FIELD) {
    compileClassVarDec();
  }
  while (current_tokenizer->keyWord() == CONSTRUCTOR || current_tokenizer->keyWord() == FUNCTION ||
         current_tokenizer->keyWord() == METHOD) {
    compileSubroutine();
  }
  outsymbol();
  num--;
  getempty();
  outf << "</class>" << endl;
}

void CompilationEngine::compileClassVarDec() {
  getempty();
  outf << "<classVarDec>" << endl;
  num++;
  outkeyword();
  no_compileType();
  no_compileVarName();
  while (current_tokenizer->symbol() == ",") {
    outsymbol();
    no_compileVarName();
  }
  outsymbol();
  num--;
  getempty();
  outf << "</classVarDec>" << endl;
}

void CompilationEngine::compileSubroutine() {
  getempty();
  outf << "<subroutineDec>" << endl;
  num++;
  outkeyword();
  if (current_tokenizer->keyWord() == VOID) {
    outkeyword();
  } else {
    no_compileType();
  }
  no_compileSubroutineName();
  outsymbol();
  compileParameterList();
  outsymbol();
  compileSubroutineBody();
  num--;
  getempty();
  outf << "</subroutineDec>" << endl;
}

void CompilationEngine::compileParameterList() {
  getempty();
  outf << "<parameterList>" << endl;
  num++;
  if (current_tokenizer->keyWord() == INT || current_tokenizer->keyWord() == CHAR ||
      current_tokenizer->keyWord() == BOOLEAN || current_tokenizer->tokenType() == IDENTIFIER) {
    no_compileType();
    no_compileVarName();
    while (current_tokenizer->symbol() != ")") {
      outsymbol();
      no_compileType();
      no_compileVarName();
    }
  }
  num--;
  getempty();
  outf << "</parameterList>" << endl;
}

void CompilationEngine::compileSubroutineBody() {
  getempty();
  outf << "<subroutineBody>" << endl;
  num++;
  outsymbol();
  while (current_tokenizer->keyWord() == VAR) {
    compileVarDec();
  }
  compileStatements();
  outsymbol();
  num--;
  getempty();
  outf << "</subroutineBody>" << endl;
}

void CompilationEngine::compileVarDec() {
  getempty();
  outf << "<varDec>" << endl;
  num++;
  outkeyword();
  no_compileType();
  no_compileVarName();
  while (current_tokenizer->symbol() == ",") {
    outsymbol();
    no_compileVarName();
  }
  outsymbol();// ;
  num--;
  getempty();
  outf << "</varDec>" << endl;
}

void CompilationEngine::compileStatements() {
  getempty();
  outf << "<statements>" << endl;
  num++;
  while (current_tokenizer->keyWord() == LET || current_tokenizer->keyWord() == IF ||
         current_tokenizer->keyWord() == WHILE || current_tokenizer->keyWord() == DO ||
         current_tokenizer->keyWord() == RETURN) {
    no_compileStatement();
  }
  num--;
  getempty();
  outf << "</statements>" << endl;
}

void CompilationEngine::compileLet() {
  getempty();
  outf << "<letStatement>" << endl;
  num++;
  outkeyword();
  no_compileVarName();
  if (current_tokenizer->symbol() == "[") {
    outsymbol();
    compileExpression();
    outsymbol();//"]"
  }
  outsymbol();
  compileExpression();
  outsymbol();
  num--;
  getempty();
  outf << "</letStatement>" << endl;
}

void CompilationEngine::compileIf() {
  getempty();
  outf << "<ifStatement>" << endl;
  num++;
  outkeyword(); // if
  outsymbol(); // (
  compileExpression();
  outsymbol(); // )
  outsymbol(); // {
  compileStatements();
  outsymbol(); // }
  if (current_tokenizer->keyWord() == ELSE) {
    outkeyword(); // else
    outsymbol(); // {
    compileStatements();
    outsymbol(); // }
  }
  num--;
  getempty();
  outf << "</ifStatement>" << endl;
}

void CompilationEngine::compileWhile() {
  getempty();
  outf << "<whileStatement>" << endl;
  num++;
  outkeyword(); //while
  outsymbol(); // (
  compileExpression();
  outsymbol(); // )
  outsymbol(); // {
  compileStatements();
  outsymbol(); // }
  num--;
  getempty();
  outf << "</whileStatement>" << endl;
}

void CompilationEngine::compileDo() {
  getempty();
  outf << "<doStatement>" << endl;
  num++;
  outkeyword(); // do
  no_compileSubroutineCall();
  outsymbol(); // ;
  num--;
  getempty();
  outf << "</doStatement>" << endl;
}

void CompilationEngine::compileReturn() {
  getempty();
  outf << "<returnStatement>" << endl;
  num++;
  outkeyword(); // return
  if (current_tokenizer->getcurrentcommand() != ";") {
    compileExpression();
  }
  outsymbol(); // ;
  num--;
  getempty();
  outf << "</returnStatement>" << endl;
}

void CompilationEngine::compileExpression() {
  getempty();
  outf << "<expression>" << endl;
  num++;
  compileTerm();
  while (current_tokenizer->getcurrentcommand() == "+" || current_tokenizer->getcurrentcommand() == "-" ||
         current_tokenizer->getcurrentcommand() == "*" || current_tokenizer->getcurrentcommand() == "/" ||
         current_tokenizer->getcurrentcommand() == "&" || current_tokenizer->getcurrentcommand() == "|" ||
         current_tokenizer->getcurrentcommand() == "<" || current_tokenizer->getcurrentcommand() == ">" ||
         current_tokenizer->getcurrentcommand() == "=") {
    no_compileOp();
    compileTerm();
  }
  num--;
  getempty();
  outf << "</expression>" << endl;
}

void CompilationEngine::compileTerm() {
  getempty();
  outf << "<term>" << endl;
  num++;
  if (current_tokenizer->tokenType() == INT_CONST) {
    outinteger();
  } else if (current_tokenizer->tokenType() == STRING_CONST) {
    outstring();
  } else if (current_tokenizer->tokenType() == KEYWORD) {
    outkeyword();
  } else if (current_tokenizer->tokenType() == IDENTIFIER) {
    outidentifier();
    if (current_tokenizer->symbol() == "[") {
      outsymbol();
      compileExpression();
      outsymbol(); // ]
    } else if (current_tokenizer->symbol() == "(" || current_tokenizer->symbol() == ".") { //subroutinename
      if (current_tokenizer->symbol() == "(") {
        outsymbol();
        compileExpressionList();
        outsymbol(); // ;
      } else if (current_tokenizer->symbol() == ".") {
        outsymbol();
        no_compileSubroutineName();
        outsymbol(); // (
        compileExpressionList();
        outsymbol(); // )
      }
    }
  } else if (current_tokenizer->symbol() == "(") {
    outsymbol();
    compileExpression();
    outsymbol(); // )
  } else if (current_tokenizer->tokenType() == SYMBOL) {
    no_compileUnaryOp();
    compileTerm();
  }
  num--;
  getempty();
  outf << "</term>" << endl;

}

void CompilationEngine::compileExpressionList() {
  getempty();
  outf << "<expressionList>" << endl;
  num++;
  if (current_tokenizer->getcurrentcommand() != ")") {
    compileExpression();

    while (current_tokenizer->symbol() == ",") {
      outsymbol();
      compileExpression();
    }
  }
  num--;
  getempty();
  outf << "</expressionList>" << endl;
}

void CompilationEngine::no_compileType() {
  if (current_tokenizer->tokenType() == KEYWORD) {
    outkeyword();
  } else if (current_tokenizer->tokenType() == IDENTIFIER) {
    outidentifier();
  }
}

void CompilationEngine::no_compileClassName() {
  outidentifier();
}

void CompilationEngine::no_compileVarName() {
  outidentifier();
}

void CompilationEngine::no_compileSubroutineName() {
  outidentifier();
}

void CompilationEngine::no_compileStatement() {
  if (current_tokenizer->keyWord() == LET) {
    compileLet();
  }
  if (current_tokenizer->keyWord() == IF) {
    compileIf();
  }
  if (current_tokenizer->keyWord() == WHILE) {
    compileWhile();
  }
  if (current_tokenizer->keyWord() == DO) {
    compileDo();
  }
  if (current_tokenizer->keyWord() == RETURN) {
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

  if (current_tokenizer->symbol() == ".") {
    outsymbol();
    no_compileSubroutineName();
  }
  outsymbol(); // (
  compileExpressionList();
  outsymbol(); // )
}

void CompilationEngine::getempty() {
  for (int i = 0; i < num; i++) {
    outf << "  ";
  }
}

void CompilationEngine::outkeyword() {
  getempty();
  outf << "<keyword> " << current_tokenizer->keyword2command(current_tokenizer->keyWord()) << " </keyword>" << endl;
  current_tokenizer->advance();
}

void CompilationEngine::outsymbol() {
  getempty();
  outf << "<symbol> " << current_tokenizer->symbol() << " </symbol>" << endl;
  current_tokenizer->advance();
}

void CompilationEngine::outinteger() {
  getempty();
  outf << "<integerConstant> " << current_tokenizer->intVal() << " </integerConstant>" << endl;
  current_tokenizer->advance();
}

void CompilationEngine::outstring() {
  getempty();
  outf << "<stringConstant> " << current_tokenizer->stringVal() << " </stringConstant>" << endl;
  current_tokenizer->advance();
}

void CompilationEngine::outidentifier() {
  getempty();
  outf << "<identifier> " << current_tokenizer->identifier() << " </identifier>" << endl;
  current_tokenizer->advance();
}





