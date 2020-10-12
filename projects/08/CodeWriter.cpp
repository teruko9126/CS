#include "CodeWriter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
using namespace std;

CodeWriter::CodeWriter(string filename)
{
  codewriter_ordernumber = 0;
  int codewriter_lastslash = filename.rfind("/");
  cout << codewriter_lastslash << endl;
  codewriter_filename = filename.substr(codewriter_lastslash + 1);
  cout << codewriter_filename << endl;
  filename += ".asm";
  outf.open(filename.c_str());
  if (!outf.is_open())
    cerr << "ファイルを開くことができませんでした:" << filename << endl;
  outf << "@256" << endl;
  outf << "D=A" << endl;
  outf << "@SP" << endl;
  outf << "M=D" << endl;
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
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M+D" << endl;
    setSP();
  }
  else if (command == "sub")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M-D" << endl;
    setSP();
  }
  else if (command == "neg")
  {
    popSP();
    outf << "M=-M" << endl;
    setSP();
  }
  else if (command == "eq")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M-D" << endl;
    outf << "D=M" << endl;
    outf << "@ZERO" << to_string(codewriter_ordernumber) << endl;
    outf << "D;JEQ" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "M=1" << endl;
    outf << "(ZERO" << to_string(codewriter_ordernumber) << ")" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "M=M-1" << endl;
    setSP();
    //stackの比較の結果をどこかに入れておいたら、それを代入するだけでいいから楽かもしれない
    codewriter_ordernumber++;
  }
  else if (command == "gt")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M-D" << endl;
    outf << "D=M" << endl;
    outf << "@GREATERTHAN" << to_string(codewriter_ordernumber) << endl;
    outf << "D;JGT" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "M=0" << endl;
    outf << "(GREATERTHAN" << to_string(codewriter_ordernumber) << ")" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "D=M" << endl;
    outf << "@GTZERO" << to_string(codewriter_ordernumber) << endl;
    outf << "D;JEQ" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "M=-1" << endl;
    outf << "(GTZERO" << to_string(codewriter_ordernumber) << ")" << endl;
    setSP();
    //stackの比較の結果をどこかに入れておいたら、それを代入するだけでいいから楽かもしれない
    codewriter_ordernumber++;
  }
  else if (command == "lt")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=D-M" << endl;
    outf << "D=M" << endl;
    outf << "@LESSTHAN" << to_string(codewriter_ordernumber) << endl;
    outf << "D;JGT" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "M=0" << endl;
    outf << "(LESSTHAN" << to_string(codewriter_ordernumber) << ")" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "D=M" << endl;
    outf << "@LTZERO" << to_string(codewriter_ordernumber) << endl;
    outf << "D;JEQ" << endl;
    outf << "@SP" << endl;
    outf << "A=M" << endl;
    outf << "M=-1" << endl;
    outf << "(LTZERO" << to_string(codewriter_ordernumber) << ")" << endl;
    setSP();
    codewriter_ordernumber++;
  }
  else if (command == "and")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M&D" << endl;
    setSP();
  }
  else if (command == "or")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M|D" << endl;
    setSP();
  }
  else if (command == "not")
  {
    popSP();
    outf << "M=!M" << endl;
    setSP();
  }
}

void CodeWriter::writePushPop(VMcommands command, string segment, int index)
{
  if (command == C_PUSH)
  {
    if (segment == "constant")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "local")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@LCL" << endl;
      outf << "A=M+D" << endl;
      outf << "D=M" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "argument")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@ARG" << endl;
      outf << "A=M+D" << endl;
      outf << "D=M" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "this")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@THIS" << endl;
      outf << "A=M+D" << endl;
      outf << "D=M" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "that")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@THAT" << endl;
      outf << "A=M+D" << endl;
      outf << "D=M" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "temp")
    {
      outf << "@" << to_string(5 + index) << endl;
      outf << "D=M" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "pointer")
    {
      outf << "@" << to_string(3 + index) << endl;
      outf << "D=M" << endl;
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else if (segment == "static")
    {
      outf << "@" << codewriter_filename << "." << to_string(index) << endl;
      outf << "D=M" << endl;
      //staticなので、その値自体がどこに位置しているのかをstackにpushしている？？？
      outf << "@SP" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
      setSP();
    }
    else
    {
      cout << "error" << endl;
    }
  }
  if (command == C_POP)
  {
    if (segment == "local")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@LCL" << endl;
      outf << "A=M+D" << endl;
      outf << "D=A" << endl;
      outf << "@R13" << endl;
      outf << "M=D" << endl;
      popSP();
      outf << "D=M" << endl;
      outf << "@R13" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
    }
    else if (segment == "argument")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@ARG" << endl;
      outf << "A=M+D" << endl;
      outf << "D=A" << endl;
      outf << "@R13" << endl;
      outf << "M=D" << endl;
      popSP();
      outf << "D=M" << endl;
      outf << "@R13" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
    }
    else if (segment == "this")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@THIS" << endl;
      outf << "A=M+D" << endl;
      outf << "D=A" << endl;
      outf << "@R13" << endl;
      outf << "M=D" << endl;
      popSP();
      outf << "D=M" << endl;
      outf << "@R13" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
    }
    else if (segment == "that")
    {
      outf << "@" << to_string(index) << endl;
      outf << "D=A" << endl;
      outf << "@THAT" << endl;
      outf << "A=M+D" << endl;
      outf << "D=A" << endl;
      outf << "@R13" << endl;
      outf << "M=D" << endl;
      popSP();
      outf << "D=M" << endl;
      outf << "@R13" << endl;
      outf << "A=M" << endl;
      outf << "M=D" << endl;
    }
    else if (segment == "temp")
    {
      popSP();
      outf << "D=M" << endl;
      outf << "@" << to_string(5 + index) << endl;
      outf << "M=D" << endl;
    }
    else if (segment == "pointer")
    {
      popSP();
      outf << "D=M" << endl;
      outf << "@" << to_string(3 + index) << endl;
      outf << "M=D" << endl;
    }
    else if (segment == "static")
    {
      popSP();
      outf << "D=M" << endl;
      outf << "@" << codewriter_filename << "." << to_string(index) << endl;
      outf << "M=D" << endl;
      //staticなので、その値自体がどこに位置しているのかをstackにpushしている？？？
    }
    else
    {
      cout << "error" << endl;
    }
  }
}

void CodeWriter::popSP(void)
{
  outf << "@SP" << endl;
  outf << "AM=M-1" << endl;
}

void CodeWriter::setSP(void)
{
  outf << "@SP" << endl;
  outf << "M=M+1" << endl;
}