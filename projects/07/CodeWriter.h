#ifndef _CODEWRITER_
#define _CODEWRITER_

#include <iostream>
#include <string>
#include <fstream>
#include "Parser.h"
using namespace std;

class CodeWriter
{
public:
  CodeWriter(string filename);
  ~CodeWriter(void);
  void setFileName(string filename);
  void writeArithmetic(string command);
  void writePushPop(VMcommands command, string segment, int index);

private:
  ofstream outf;
  string codewriter_filename;
};

#endif
