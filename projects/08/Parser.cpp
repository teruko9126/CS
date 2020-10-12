#include "Parser.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

Parser::Parser(string filename)
{
  int parser_lastslash = filename.rfind("/");
  parser_filename = filename.substr(parser_lastslash + 1);
  filename += ".vm";
  inf.open(filename.c_str());
  if (!inf.is_open())
    cerr << "ファイルを開くことができませんでした:" << filename << endl;
}

Parser::~Parser()
{
  if (inf.is_open())
  {
    inf.close();
  }
}

bool Parser::hasMoreCommands(void)
{
  return !inf.eof();
}

void Parser::advance(void)
{
  if (hasMoreCommands())
  {
    getline(inf, parser_currentcommand);
    while ((parser_currentcommand.substr(0, 2) == "//" || parser_currentcommand == "") && hasMoreCommands())
      getline(inf, parser_currentcommand);
  }
}

VMcommands Parser::commandType(void)
{
  if (parser_currentcommand.substr(0, 3) == "add" || parser_currentcommand.substr(0, 3) == "sub" || parser_currentcommand.substr(0, 3) == "neg" || parser_currentcommand.substr(0, 2) == "eq" || parser_currentcommand.substr(0, 2) == "gt" || parser_currentcommand.substr(0, 2) == "lt" || parser_currentcommand.substr(0, 3) == "and" || parser_currentcommand.substr(0, 3) == "or" || parser_currentcommand.substr(0, 3) == "not")
    return C_ARITHMETIC;
  else if (parser_currentcommand.substr(0, 4) == "push")
    return C_PUSH;
  else if (parser_currentcommand.substr(0, 3) == "pop")
    return C_POP;
  else if (parser_currentcommand.substr(0, 5) == "label")
    return C_LABEL;
  else if (parser_currentcommand.substr(0, 4) == "goto")
    return C_GOTO;
  else if (parser_currentcommand.substr(0, 7) == "if-goto")
    return C_IF;
  else if (parser_currentcommand.substr(0, 8) == "function")
    return C_FUNCTION;
  else if (parser_currentcommand.substr(0, 6) == "return")
    return C_RETURN;
  else if (parser_currentcommand.substr(0, 4) == "call")
    return C_CALL;
}

string Parser::arg1(void)
{
  VMcommands ct = commandType();
  if (ct != C_RETURN)
  {
    string a1 = "";
    if (ct == C_ARITHMETIC)
    {
      a1 = parser_currentcommand.substr(0, parser_currentcommand.size() - 1);
    }
    else
    {
      int firstspace = parser_currentcommand.find(" ");
      int secondspace = parser_currentcommand.find(" ", firstspace + 1);
      a1 = parser_currentcommand.substr(firstspace + 1, secondspace - firstspace - 1);
    }
    return a1;
  }
}

int Parser::arg2(void)
{
  VMcommands ct = commandType();
  if (ct == C_PUSH || ct == C_POP || ct == C_FUNCTION || ct == C_CALL)
  {
    int firstspace = parser_currentcommand.find(" ");
    int secondspace = parser_currentcommand.find(" ", firstspace + 1);
    int thirdspace = parser_currentcommand.find(" ", secondspace + 1);
    string a2 = parser_currentcommand.substr(secondspace + 1, thirdspace - secondspace - 1);
    int a2int = stoi(a2);
    return a2int;
  }
}
