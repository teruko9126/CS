#include "CompilationEngine.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

CompilationEngine::CompilationEngine(JackTokenizer *tokenizer, Symboltable *symboltable, VMWriter *vmwriter,
                                     string filename) {
  loopnumber = 0;
  stringnumber = 0;
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
  } else if (TOKEN->tokenType() == IDENTIFIER) {
    type = TOKEN->identifier();
  }

  TOKEN->advance();
  while (TOKEN->getcurrentcommand() != ";") {
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

void CompilationEngine::compileSubroutine() {
  SYM->startSubroutine();

  subroutineType = TOKEN->keyWord();
  //"this"という名前で常にアドレスを参照可能
  if (subroutineType == METHOD) {
    SYM->define("this", classname, symboltable::ARG);
  }
  TOKEN->advance();

  string returntype; //!のちに返り値について考えるときに使う?????
  if (TOKEN->tokenType() == KEYWORD) {
    returntype = CEhelper::keyword2string(TOKEN->keyWord());
  } else if (TOKEN->tokenType() == IDENTIFIER) {
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
  string name, type;
  symboltable::Kind kind = symboltable::ARG;
  bool truetype = true;
  while (TOKEN->getcurrentcommand() != ")") {
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
      case SYMBOL:
        if (TOKEN->symbol() == ",")
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

  VM->writeFunction(subroutinename, nLocals);

  //functionは実際にその中身を動かす
  //constructorは初期化
  //methodはその中身をその場で動かすわけではない、もし動かしたらこういうことがおこるよ、的な説明書
  if (subroutineType == CONSTRUCTOR) {
    VM->writePush(vmwriter::CONST, SYM->VarCount(symboltable::FIELD));
    VM->writeCall("Memory.alloc", 1);
    VM->writePop(vmwriter::POINTER, 0);
  } else if (subroutineType == METHOD) {
    //一番最初のargumentにアドレスを渡している
    VM->writePush(vmwriter::ARG, 0);
    VM->writePop(vmwriter::POINTER, 0);
  }
  compileStatements();

  assert(TOKEN->symbol() == "}");
  TOKEN->advance();
}

void CompilationEngine::compileVarDec() {
  string name, type;
  symboltable::Kind kind = symboltable::VAR;

  assert(TOKEN->keyWord() == VAR);
  TOKEN->advance();

  //this define type
  if (TOKEN->tokenType() == KEYWORD) {
    type = CEhelper::keyword2string(TOKEN->keyWord());
  } else if (TOKEN->tokenType() == IDENTIFIER) {
    type = TOKEN->identifier();
  }
  TOKEN->advance();

  while (TOKEN->getcurrentcommand() != ";") {
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
  while (TOKEN->keyWord() == LET || TOKEN->keyWord() == IF || TOKEN->keyWord() == WHILE || TOKEN->keyWord() == DO ||
         TOKEN->keyWord() == RETURN) {
    switch (TOKEN->keyWord()) {
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
  TOKEN->advance();

  string letname = TOKEN->identifier();
  bool isarray = false;
  TOKEN->advance();

  if (TOKEN->getcurrentcommand() == "[") {
    isarray = true;
    TOKEN->advance();

    compileExpression();

    VM->writePush(CEhelper::kind2Segment(SYM->kindOf(letname)), SYM->indexOf(letname));
    VM->writeArithmetic(vmwriter::ADD);

    assert(TOKEN->symbol() == "]");
    TOKEN->advance();
  }

  assert(TOKEN->symbol() == "=");
  TOKEN->advance();

  compileExpression();

  if (isarray) {
    VM->writePop(vmwriter::TEMP, 0);
    VM->writePop(vmwriter::POINTER, 1);
    VM->writePush(vmwriter::TEMP, 0);
    //thatはpointer[1]の値をアドレスとみて、その中の値を変更させる
    VM->writePop(vmwriter::THAT, 0);

  } else {
    VM->writePop(CEhelper::kind2Segment(SYM->kindOf(letname)), SYM->indexOf(letname));
  }

  assert(TOKEN->symbol() == ";");
  TOKEN->advance();
}

void CompilationEngine::compileIf() {
  string iftrue = "IFTRUE" + to_string(loopnumber);
  string iffalse = "IFFALSE" + to_string(loopnumber);
  loopnumber++;

  TOKEN->advance();

  assert(TOKEN->symbol() == "(");
  TOKEN->advance();

  compileExpression();
  VM->writeArithmetic(vmwriter::NOT);

  assert(TOKEN->symbol() == ")");
  VM->writeIf(iftrue);
  TOKEN->advance();

  assert(TOKEN->symbol() == "{");
  TOKEN->advance();

  compileStatements();

  assert(TOKEN->symbol() == "}");
  VM->writeGoto(iffalse);
  TOKEN->advance();

  VM->writeLabel(iftrue);
  if (TOKEN->keyWord() == ELSE) {
    TOKEN->advance();

    assert(TOKEN->symbol() == "{");
    TOKEN->advance();

    compileStatements();

    assert(TOKEN->symbol() == "}");
    TOKEN->advance();
  }
  VM->writeLabel(iffalse);
}

void CompilationEngine::compileWhile() {
  string whiletrue = "WHILETRUE" + to_string(loopnumber);
  string whilefalse = "WHILEFALSE" + to_string(loopnumber);
  loopnumber++;

  assert(TOKEN->keyWord() == WHILE);
  VM->writeLabel(whiletrue);
  TOKEN->advance();

  assert(TOKEN->symbol() == "(");
  TOKEN->advance();

  compileExpression();
  VM->writeArithmetic(vmwriter::NOT);
  //!if-gotoは0でなければ飛ぶ！

  assert(TOKEN->symbol() == ")");
  VM->writeIf(whilefalse);
  TOKEN->advance();

  assert(TOKEN->symbol() == "{");
  TOKEN->advance();

  compileStatements();
  VM->writeGoto(whiletrue);

  assert(TOKEN->symbol() == "}");
  VM->writeLabel(whilefalse);
  TOKEN->advance();
}

void CompilationEngine::compileDo() {
  string name1, name2;
  assert(TOKEN->keyWord() == DO);
  TOKEN->advance();

  name1 = TOKEN->identifier();
  TOKEN->advance();

  if (TOKEN->symbol() == "(") {
    TOKEN->advance();
    VM->writePush(vmwriter::POINTER, 0);
    numArgs++;

    name2 = name1;
    name1 = classname;

    //TODO　なぜpointerをpushする？？？？？今はまだここ通らないのでスルーする
  } else if (TOKEN->symbol() == ".") {
    TOKEN->advance();

    name2 = TOKEN->identifier();
    TOKEN->advance();

    //!名前から構造体と判明した場合、構造体のアドレスをが引数として必要なので、そのアドレスを最初の引数として渡す
    if (SYM->kindOf(name1) != symboltable::NONE) {
      VM->writePush(CEhelper::kind2Segment(SYM->kindOf(name1)), SYM->indexOf(name1));
      numArgs++;
      name1 = SYM->typeOf(name1);
    }
    assert(TOKEN->symbol() == "(");
    TOKEN->advance();

  }

  compileExpressionList();
  VM->writeCall(name1 + "." + name2, numArgs);
  numArgs = 0;

  assert(TOKEN->symbol() == ")");
  TOKEN->advance();

  //!スタックの一番上は返り値が入っているのでとりあえずtempにどかす
  VM->writePop(vmwriter::TEMP, 0);

  assert(TOKEN->symbol() == ";");
  TOKEN->advance();
}

void CompilationEngine::compileReturn() {
  assert(TOKEN->keyWord() == RETURN);
  TOKEN->advance();

  if (TOKEN->getcurrentcommand() != ";") {
    compileExpression();
  } else {
    VM->writePush(vmwriter::CONST, 0);
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
         TOKEN->getcurrentcommand() == "=") {

    string op = TOKEN->symbol();
    TOKEN->advance();

    compileTerm();
    if (op == "+")
      VM->writeArithmetic(vmwriter::ADD);
    else if (op == "-")
      VM->writeArithmetic(vmwriter::SUB);
    else if (op == "*")
      VM->writeCall("Math.multiply", 2);
    else if (op == "/")
      VM->writeCall("Math.divide", 2);
    else if (op == "&")
      VM->writeArithmetic(vmwriter::AND);
    else if (op == "|")
      VM->writeArithmetic(vmwriter::OR);
    else if (op == "<")
      VM->writeArithmetic(vmwriter::LT);
    else if (op == ">")
      VM->writeArithmetic(vmwriter::GT);
    else if (op == "=")
      VM->writeArithmetic(vmwriter::EQ);
    else if (op == "~")
      VM->writeArithmetic(vmwriter::NOT);
  }
}

void CompilationEngine::compileTerm() {
  string subname1, subname2;
  switch (TOKEN->tokenType()) {
    case KEYWORD:
      if (TOKEN->keyWord() == TRUE) {
        VM->writePush(vmwriter::CONST, 0);
        VM->writeArithmetic(vmwriter::NOT);
      } else if (TOKEN->keyWord() == FALSE) {
        VM->writePush(vmwriter::CONST, 0);
      } else if (TOKEN->keyWord() == THIS) {
        VM->writePush(vmwriter::POINTER, 0);
      }
      TOKEN->advance();
      break;
    case SYMBOL:
      if (TOKEN->symbol() == "(") {
        TOKEN->advance();

        compileExpression();

        assert(TOKEN->symbol() == ")");
        TOKEN->advance();
      } else {
        string unOP = TOKEN->symbol();
        TOKEN->advance();

        compileTerm();

        if (unOP == "-")
          VM->writeArithmetic(vmwriter::NEG);
        else if (unOP == "~")
          VM->writeArithmetic(vmwriter::NOT);
      }
      break;
    case INT_CONST:
      VM->writePush(vmwriter::CONST, TOKEN->intVal());
      TOKEN->advance();
      break;
    case STRING_CONST:
      usestring = TOKEN->stringVal();
      stringnumber = usestring.size();
      VM->writePush(vmwriter::CONST, stringnumber);
      VM->writeCall("String.new", 1);
      for (int i = 0; i < stringnumber; i++) {
        VM->writePush(vmwriter::CONST, usestring[i]);
        VM->writeCall("String.appendChar", 2);
      }

      TOKEN->advance();
      break;
    case IDENTIFIER:
      subname1 = TOKEN->identifier();
      TOKEN->advance();

      if (TOKEN->getcurrentcommand() == ".") {
        TOKEN->advance();

        subname2 = TOKEN->identifier();
        TOKEN->advance();

        assert(TOKEN->symbol() == "(");
        TOKEN->advance();

        compileExpressionList();

        if (SYM->kindOf(subname1) != symboltable::NONE) {
          VM->writePush(CEhelper::kind2Segment(SYM->kindOf(subname1)), SYM->indexOf(subname1));
          numArgs++;
          subname1 = SYM->typeOf(subname1);
        }

        VM->writeCall(subname1 + "." + subname2, numArgs);
        numArgs = 0;

        assert(TOKEN->symbol() == ")");
        TOKEN->advance();
      } else if (TOKEN->symbol() == "[") {
        TOKEN->advance();

        compileExpression();
        VM->writePush(CEhelper::kind2Segment(SYM->kindOf(subname1)), SYM->indexOf(subname1));
        VM->writeArithmetic(vmwriter::ADD);

        assert(TOKEN->symbol() == "]");
        VM->writePop(vmwriter::POINTER, 1);
        VM->writePush(vmwriter::THAT, 0);
        TOKEN->advance();
      } else {
        VM->writePush(CEhelper::kind2Segment(SYM->kindOf(subname1)), SYM->indexOf(subname1));
      }
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
