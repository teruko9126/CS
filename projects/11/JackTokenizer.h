#ifndef _JACKTOKENIZER_
#define _JACKTOKENIZER_

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>

using namespace std;

enum TokenType {
  KEYWORD,
  SYMBOL,
  IDENTIFIER,
  INT_CONST,
  STRING_CONST
};

enum KeyWord {
  CLASS,
  METHOD,
  FUNCTION,
  CONSTRUCTOR,
  INT,
  BOOLEAN,
  CHAR,
  VOID,
  VAR,
  STATIC,
  FIELD,
  LET,
  DO,
  IF,
  ELSE,
  WHILE,
  RETURN,
  TRUE,
  FALSE,
  WORDNULL,
  THIS
};

class JackTokenizer {
public:
  JackTokenizer(string filename);

  ~JackTokenizer(void);

  bool hasMoreTokens(void);

  void advance(void);

  TokenType tokenType(void);

  KeyWord keyWord(void);

  string symbol(void);

  string identifier(void);

  int intVal(void);

  string stringVal(void);

  string keyword2command(KeyWord input_keyword);

  string getcurrentcommand(void);

private:
  string returnstring;
  string currentcommand;
  ifstream inf;
  string tokenize_filename;
};

#endif
