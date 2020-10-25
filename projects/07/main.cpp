#include "CodeWriter.h"
#include "Parser.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  string filename = argv[1];
  int dot = filename.find(".");
  filename = filename.substr(0, dot);

  Parser parser1(filename);
  CodeWriter codewriter1(filename);
  while (parser1.hasMoreCommands())
  {
    parser1.advance();

    if (parser1.commandType() == C_PUSH)
    {
      codewriter1.writePushPop(parser1.commandType(), parser1.arg1(), parser1.arg2());
    }
    else if (parser1.commandType() == C_POP)
    {
      codewriter1.writePushPop(parser1.commandType(), parser1.arg1(), parser1.arg2());
    }
    else if (parser1.commandType() == C_ARITHMETIC)
    {
      codewriter1.writeArithmetic(parser1.arg1());
    }
  }
}
