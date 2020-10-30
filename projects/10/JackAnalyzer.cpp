#include "JackTokenizer.h"
#include "preexec.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cctype>

//main.jackを読み込んでtokenizerのためのファイルであるmainpre.jackを作成する
//maintokenizer.jackをjacktokenizerで読み込む
//最終的な結果としてmain.xmlを作成する

int main(int argc, char *argv[]) {
  string filename = argv[1];
  // .jack付ける

  preexec preexec1(filename);


  int dot = filename.find(".");
  string prefilename = filename.substr(0, dot) + "pre.jack";
  ofstream outf1(prefilename.c_str());

  while (preexec1.hasmorestring()) {
    preexec1.advance();
    preexec1.findsymbol();

    outf1 << preexec1.getstring();
  }

  outf1.close();

  JackTokenizer tokenizer1(prefilename.c_str());

  string tokenfilename = filename.substr(0, dot) + "token.xml";
  ofstream outf2(tokenfilename.c_str());

  outf2 << "<tokens>" << endl;

  tokenizer1.advance();

  while (tokenizer1.hasMoreTokens()) {

    if (tokenizer1.tokenType() == KEYWORD) {
      outf2 << "<keyword> ";
      if (tokenizer1.keyWord() == CLASS) outf2 << "class </keyword>" << endl;
      else if (tokenizer1.keyWord() == METHOD) outf2 << "method </keyword>" << endl;
      else if (tokenizer1.keyWord() == FUNCTION) outf2 << "function </keyword>" << endl;
      else if (tokenizer1.keyWord() == CONSTRUCTOR) outf2 << "constructor </keyword>" << endl;
      else if (tokenizer1.keyWord() == INT) outf2 << "int </keyword>" << endl;
      else if (tokenizer1.keyWord() == BOOLEAN) outf2 << "boolean </keyword>" << endl;
      else if (tokenizer1.keyWord() == CHAR) outf2 << "char </keyword>" << endl;
      else if (tokenizer1.keyWord() == VOID) outf2 << "void </keyword>" << endl;
      else if (tokenizer1.keyWord() == VAR) outf2 << "var </keyword>" << endl;
      else if (tokenizer1.keyWord() == STATIC) outf2 << "static </keyword>" << endl;
      else if (tokenizer1.keyWord() == FIELD) outf2 << "field </keyword>" << endl;
      else if (tokenizer1.keyWord() == LET) outf2 << "let </keyword>" << endl;
      else if (tokenizer1.keyWord() == DO) outf2 << "do </keyword>" << endl;
      else if (tokenizer1.keyWord() == IF) outf2 << "if </keyword>" << endl;
      else if (tokenizer1.keyWord() == ELSE) outf2 << "else </keyword>" << endl;
      else if (tokenizer1.keyWord() == WHILE) outf2 << "while </keyword>" << endl;
      else if (tokenizer1.keyWord() == RETURN) outf2 << "return </keyword>" << endl;
      else if (tokenizer1.keyWord() == TRUE) outf2 << "true </keyword>" << endl;
      else if (tokenizer1.keyWord() == FALSE) outf2 << "false </keyword>" << endl;
      else if (tokenizer1.keyWord() == WORDNULL) outf2 << "null </keyword>" << endl;
      else if (tokenizer1.keyWord() == THIS) outf2 << "this </keyword>" << endl;
    } else if (tokenizer1.tokenType() == SYMBOL) {
      cout << tokenizer1.symbol() << endl;
      outf2 << "<symbol> " << tokenizer1.symbol() << " </symbol>" << endl;
    } else if (tokenizer1.tokenType() == INT_CONST) {
      outf2 << "<integerConstant> " << tokenizer1.intVal() << " </integerConstant>" << endl;
    } else if (tokenizer1.tokenType() == STRING_CONST) {
      outf2 << "<stringConstant> " << tokenizer1.stringVal() << " </stringConstant>" << endl;
    } else if (tokenizer1.tokenType() == IDENTIFIER) {
      outf2 << "<identifier> " << tokenizer1.identifier() << " </identifier>" << endl;
    }

    tokenizer1.advance();
  }
  outf2 << "</tokens>" << endl;
  outf2.close();
}