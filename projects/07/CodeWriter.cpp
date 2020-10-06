#include "CodeWriter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
using namespace std;

CodeWriter::CodeWriter(string filename)
{
  codewriter_filename = filename;
  filename += ".asm";
  outf.open(filename.c_str());
  if (!outf.is_open())
    cerr << "ファイルを開くことができませんでした:" << filename << endl;
}

CodeWriter::~CodeWriter()
{
  if (outf.is_open())
    outf.close();
}

void CodeWriter::writeArithmetic(string command)
{
  if (command == "add")
  {
    outf << "@SP" << endl;
    outf << "AM=M-1" << endl;
    outf << "D=M" << endl;
    outf << "@SP" << endl;
    outf << "AM=M-1" << endl;
    outf << "M=D+M" << endl;
    outf << "@SP" << endl;
    outf << "M=M+1" << endl;
  }
  if (command == "eq")
  {
    outf << "@SP" << endl;
    outf << "AM=M-1" << endl;
    outf << "D=M" << endl;
    outf << "@SP" << endl;
    outf << "AM=M-1" << endl;
    outf << "M=D&M" << endl;
    outf << "@SP" << endl;
    outf << "M=M+1" << endl;
  }
  if (command == "lt")
  {
    outf << "@SP" << endl;
    outf << "AM=M-1" << endl;
    outf << "D=M" << endl;
    outf << "@SP" << endl;
    outf << "AM=M-1" << endl;
    outf << "M=D&M" << endl;
    outf << "@SP" << endl;
    outf << "M=M+1" << endl;
  }
}

void CodeWriter::writePushPop(VMcommands command, string segment, int index)
{
  if (command == C_PUSH)
  {
    if (segment == "constant")
    {
      outf << "@" << index << endl;
      outf << "D=A" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      outf << "@SP" << endl;
      outf << "M=M+1" << endl;
    }
    else
    {
      cout << "error" << endl;
    }
  }
}