#include "CodeWriter.h"
#include "Parser.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{

  string filename = argv[1];
  CodeWriter codewriter1(filename);

  for (int i = 2; i < argc; i++)
  {
    int num = 0;
    filename = argv[i];
    int dot = filename.find(".");
    filename.substr(0, dot);
    Parser parser1(filename);
    codewriter1.setFileName(filename);

    codewriter1.setempty();

    while (parser1.hasMoreCommands())
    {

      parser1.advance();
      cout << parser1.Getparserstring() << endl;
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
      else if (parser1.commandType() == C_IF)
      {
        codewriter1.writeIf(parser1.arg1());
      }
      else if (parser1.commandType() == C_LABEL)
      {
        codewriter1.writeLabel(parser1.arg1());
      }
      else if (parser1.commandType() == C_GOTO)
      {
        codewriter1.writeGoto(parser1.arg1());
      }
      else if (parser1.commandType() == C_FUNCTION)
      {
        codewriter1.writeFunction(parser1.arg1(), parser1.arg2());
      }
      else if (parser1.commandType() == C_RETURN)
      {
        codewriter1.writeReturn();
      }
      else if (parser1.commandType() == C_CALL)
      {
        codewriter1.writeCall(parser1.arg1(), parser1.arg2());
      }

      codewriter1.setempty();
    }
  }
}
