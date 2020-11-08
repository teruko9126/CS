#include "JackTokenizer.h"
#include "JackTokenizer.cpp"
#include "CompilationEngine.h"
#include "CompilationEngine.cpp"
#include "SymbolTable.h"
#include "SymbolTable.cpp"
#include "VMWriter.h"
#include "VMWriter.cpp"
#include "preexec.h"
#include "preexec.cpp"
#include <fstream>
#include <string>
#include <iostream>
#include <cctype>

int main(int argc, char *argv[])
{
  string filename = argv[1];
  // .jack付ける

  preexec preexec1(filename);

  int dot = filename.find(".");
  string prefilename = filename.substr(0, dot) + "pre.xml";
  ofstream outf1(prefilename.c_str());

  while (preexec1.hasmorestring())
  {
    preexec1.advance();
    preexec1.findsymbol();

    outf1 << preexec1.getstring();
  }

  int a = 3;

  outf1.close();
  JackTokenizer tokenizer1(prefilename);
  Symboltable symboltable1;
  VMWriter vmwriter1(filename);
  CompilationEngine compilationEngine1(&tokenizer1, &symboltable1, &vmwriter1, prefilename);
}