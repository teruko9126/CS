#include "JackTokenizer.h"

using namespace std;

JackTokenizer::JackTokenizer(string filename) {
  tokenize_filename = filename;
  inf.open(filename.c_str());
  if (!inf.is_open())
    cout << "can't open tokenizer:" << filename << endl;
}

JackTokenizer::~JackTokenizer() {
  if (inf.is_open())
    inf.close();
}

bool JackTokenizer::hasMoreTokens() {
  return !inf.eof();
}

void JackTokenizer::advance() {
  if (hasMoreTokens()) {
    inf >> currentcommand;
  }
}

TokenType JackTokenizer::tokenType() {
  if (currentcommand == "class" || currentcommand == "constructor" ||
      currentcommand == "function" || currentcommand == "method" ||
      currentcommand == "field" || currentcommand == "static" ||
      currentcommand == "var" || currentcommand == "int" ||
      currentcommand == "char" || currentcommand == "boolean" ||
      currentcommand == "void" || currentcommand == "true" ||
      currentcommand == "false" || currentcommand == "null" ||
      currentcommand == "this" || currentcommand == "let" ||
      currentcommand == "do" || currentcommand == "if" ||
      currentcommand == "else" || currentcommand == "while" ||
      currentcommand == "return") {
    return KEYWORD;
  } else if (currentcommand[0] == '{' || currentcommand[0] == '}' ||
             currentcommand[0] == '(' || currentcommand[0] == ')' ||
             currentcommand[0] == '[' || currentcommand[0] == ']' ||
             currentcommand[0] == '.' || currentcommand[0] == ',' ||
             currentcommand[0] == ';' || currentcommand[0] == '+' ||
             currentcommand[0] == '-' || currentcommand[0] == '*' ||
             currentcommand[0] == '/' || currentcommand[0] == '&' ||
             currentcommand[0] == '|' || currentcommand[0] == '<' ||
             currentcommand[0] == '>' || currentcommand[0] == '=' ||
             currentcommand[0] == '~') {
    return SYMBOL;
  } else if (isdigit(currentcommand[0])) {
    return INT_CONST;
  } else if (currentcommand[0] == '"') {
    return STRING_CONST;
  } else if (!isdigit(currentcommand[0])) {
    return IDENTIFIER;
  } else {
    cerr << "error:tokentype" << endl;
  }
}

KeyWord JackTokenizer::keyWord() {
  if (tokenType() == KEYWORD) {
    if (currentcommand == "class") return CLASS;
    if (currentcommand == "method") return METHOD;
    if (currentcommand == "function") return FUNCTION;
    if (currentcommand == "constructor") return CONSTRUCTOR;
    if (currentcommand == "int") return INT;
    if (currentcommand == "boolean") return BOOLEAN;
    if (currentcommand == "char") return CHAR;
    if (currentcommand == "void") return VOID;
    if (currentcommand == "var") return VAR;
    if (currentcommand == "static") return STATIC;
    if (currentcommand == "field") return FIELD;
    if (currentcommand == "let") return LET;
    if (currentcommand == "do") return DO;
    if (currentcommand == "if") return IF;
    if (currentcommand == "else") return ELSE;
    if (currentcommand == "while") return WHILE;
    if (currentcommand == "return") return RETURN;
    if (currentcommand == "true") return TRUE;
    if (currentcommand == "false") return FALSE;
    if (currentcommand == "null") return WORDNULL;
    if (currentcommand == "this") return THIS;
  }
}

string JackTokenizer::symbol() {
  if (tokenType() == SYMBOL) {
    return currentcommand;
  }
}

string JackTokenizer::identifier() {
  if (tokenType() == IDENTIFIER) return currentcommand;
}

int JackTokenizer::intVal() {
  if (tokenType() == INT_CONST) return stoi(currentcommand);
}

string JackTokenizer::stringVal() {
  string currentstring = "";
  if (tokenType() == STRING_CONST) {
    advance();
    while (currentcommand[0] != '"') {
      cout << currentstring << endl;
      currentstring += currentcommand + " ";
      advance();
    }
  }
  return currentstring;
}

string JackTokenizer::keyword2command(KeyWord input_keyword) {
  if (input_keyword == CLASS) return "class";
  if (input_keyword == METHOD) return "method";
  if (input_keyword == FUNCTION) return "function";
  if (input_keyword == CONSTRUCTOR) return "constructor";
  if (input_keyword == INT) return "int";
  if (input_keyword == BOOLEAN) return "boolean";
  if (input_keyword == CHAR) return "char";
  if (input_keyword == VOID) return "void";
  if (input_keyword == VAR) return "var";
  if (input_keyword == STATIC) return "static";
  if (input_keyword == FIELD) return "field";
  if (input_keyword == LET) return "let";
  if (input_keyword == DO) return "do";
  if (input_keyword == IF) return "if";
  if (input_keyword == ELSE) return "else";
  if (input_keyword == WHILE) return "while";
  if (input_keyword == RETURN) return "return";
  if (input_keyword == TRUE) return "true";
  if (input_keyword == FALSE) return "false";
  if (input_keyword == WORDNULL) return "null";
  if (input_keyword == THIS) return "this";
}

string JackTokenizer::getcurrentcommand() {
  return currentcommand;
}




