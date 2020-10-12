#ifndef _PERSER_
//このhファイルに関しては一回だけ読み込めばいいため、ifndefで囲んでいる
#define _PERSER_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum VMcommands
{
  C_ARITHMETIC,
  C_PUSH,
  C_POP,
  C_LABEL,
  C_GOTO,
  C_IF,
  C_FUNCTION,
  C_RETURN,
  C_CALL
};

class Parser
{
public:
  //constructer
  Parser(string filename);
  //destructer
  ~Parser(void);

  //methods
  bool hasMoreCommands(void);
  void advance(void);
  VMcommands commandType(void);
  string arg1(void);
  int arg2(void);

private:
  ifstream inf;
  string parser_filename;
  string parser_currentcommand;
};

#endif