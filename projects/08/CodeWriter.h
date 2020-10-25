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
  void setempty(void);
  void setFileName(string filename);
  void writeArithmetic(string command);
  void writePushPop(VMcommands command, string segment, int index);
  void writeInit(void);
  void writeLabel(string label);
  void writeGoto(string label);
  void writeIf(string lavel);
  void writeCall(string functionName, int numArgs);
  void writeReturn(void);
  void writeFunction(string functionName, int numLocals);

private:
  ofstream outf;
  string codewriter_filename;
  int codewriter_ordernumber;
  int codewriter_lastslash;
  int codewriter_start;
  int codewriter_number;
  void popSP(void);
  void setSP(void);
};

#endif
