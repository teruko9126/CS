#include "CodeWriter.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

CodeWriter::CodeWriter(string filename)
{
  codewriter_number = 0;
  codewriter_filename = filename;
  filename += ".asm";
  outf.open(filename.c_str());
  if (!outf.is_open())
    cerr << "ファイルを開くことができませんでした:" << filename << endl;
  writeInit();
}

CodeWriter::~CodeWriter()
{
  if (outf.is_open())
    outf.close();
}

void CodeWriter::setempty(void)
{
  outf << endl;
}

void CodeWriter::setFileName(string filename)
{
  int codewriter_lastslash = filename.rfind("/");
  codewriter_filename = filename.substr(codewriter_lastslash + 1);
}

void CodeWriter::writeInit(void)
{
  outf << "@256" << endl;
  outf << "D=A" << endl;
  outf << "@SP" << endl;
  outf << "M=D" << endl;
  writeCall("Sys.init", 0);
}

void CodeWriter::writeArithmetic(string command)
{
  if (command.substr(0, 3) == "add")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M+D" << endl;
    setSP();
  }
  else if (command.substr(0, 3) == "sub")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M-D" << endl;
    setSP();
  }
  else if (command.substr(0, 3) == "neg")
  {
    popSP();
    outf << "M=-M" << endl;
    setSP();
  }
  else if (command.substr(0, 2) == "eq")
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
  else if (command.substr(0, 2) == "gt")
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
  else if (command.substr(0, 2) == "lt")
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
  else if (command.substr(0, 3) == "and")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M&D" << endl;
    setSP();
  }
  else if (command.substr(0, 2) == "or")
  {
    popSP();
    outf << "D=M" << endl;
    popSP();
    outf << "M=M|D" << endl;
    setSP();
  }
  else if (command.substr(0, 3) == "not")
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

void CodeWriter::writeLabel(string label)
{
  outf << "(" << codewriter_filename << "$" << label << ")" << endl;
}

void CodeWriter::writeIf(string label)
{
  popSP();
  outf << "D=M" << endl;
  outf << "@" << codewriter_filename << "$" << label << endl;
  outf << "D;JNE" << endl;
}

void CodeWriter::writeGoto(string label)
{
  outf << "@" << codewriter_filename << "$" << label << endl;
  outf << "0;JMP" << endl;
}

void CodeWriter::writeFunction(string functionName, int numLocals)
{
  outf << "(" << functionName << ")" << endl;
  for (int i = 0; i < numLocals; i++)
  {
    writePushPop(C_PUSH, "constant", 0);
  }
}

void CodeWriter::writeReturn(void)
{
  // 関数の中で関数を使う可能性があるので同じ名前のアドレスを中で定義してはいけない！
  //　どのようにして別物の名前を定義する？？

  outf << "@LCL" << endl; //address of local
  outf << "D=M" << endl;
  outf << "@FRAME" << endl;
  outf << "M=D" << endl;

  outf << "@5" << endl;
  outf << "A=D-A" << endl;
  outf << "D=M" << endl;
  outf << "@RET" << endl;
  outf << "M=D" << endl;

  popSP();
  outf << "D=M" << endl;
  outf << "@ARG" << endl;
  outf << "A=M" << endl;
  outf << "M=D" << endl;

  outf << "@ARG" << endl;
  outf << "D=M" << endl;
  outf << "@SP" << endl;
  outf << "M=D+1" << endl;

  outf << "@FRAME" << endl;
  outf << "D=M" << endl;
  outf << "@1" << endl;
  outf << "A=D-A" << endl;
  outf << "D=M" << endl;
  outf << "@THAT" << endl;
  outf << "M=D" << endl;

  outf << "@FRAME" << endl;
  outf << "D=M" << endl;
  outf << "@2" << endl;
  outf << "A=D-A" << endl;
  outf << "D=M" << endl;
  outf << "@THIS" << endl;
  outf << "M=D" << endl;

  outf << "@FRAME" << endl;
  outf << "D=M" << endl;
  outf << "@3" << endl;
  outf << "A=D-A" << endl;
  outf << "D=M" << endl;
  outf << "@ARG" << endl;
  outf << "M=D" << endl;

  outf << "@FRAME" << endl;
  outf << "D=M" << endl;
  outf << "@4" << endl;
  outf << "A=D-A" << endl;
  outf << "D=M" << endl;
  outf << "@LCL" << endl;
  outf << "M=D" << endl;

  outf << "@RET" << endl;
  outf << "A=M" << endl;
  outf << "0;JMP" << endl;
}

void CodeWriter::writeCall(string functionName, int numLocals)
{
  outf << "@RETURN" << codewriter_number << endl;
  outf << "D=A" << endl;
  outf << "@SP" << endl;
  outf << "A=M" << endl;
  outf << "M=D" << endl;
  outf << "@SP" << endl;
  outf << "M=M+1" << endl;

  outf << "@LCL" << endl;
  outf << "D=M" << endl;
  outf << "@SP" << endl;
  outf << "A=M" << endl;
  outf << "M=D" << endl;
  outf << "@SP" << endl;
  outf << "M=M+1" << endl;

  outf << "@ARG" << endl;
  outf << "D=M" << endl;
  outf << "@SP" << endl;
  outf << "A=M" << endl;
  outf << "M=D" << endl;
  outf << "@SP" << endl;
  outf << "M=M+1" << endl;

  outf << "@THIS" << endl;
  outf << "D=M" << endl;
  outf << "@SP" << endl;
  outf << "A=M" << endl;
  outf << "M=D" << endl;
  outf << "@SP" << endl;
  outf << "M=M+1" << endl;

  outf << "@THAT" << endl;
  outf << "D=M" << endl;
  outf << "@SP" << endl;
  outf << "A=M" << endl;
  outf << "M=D" << endl;
  outf << "@SP" << endl;
  outf << "M=M+1" << endl;

  outf << "@SP" << endl;
  outf << "D=M" << endl;
  outf << "@5" << endl;
  outf << "D=D-A" << endl;
  outf << "@" << to_string(numLocals) << endl;
  outf << "D=D-A" << endl;
  outf << "@ARG" << endl;
  outf << "M=D" << endl;

  outf << "@SP" << endl;
  outf << "D=M" << endl;
  outf << "@LCL" << endl;
  outf << "M=D" << endl;

  outf << "@" << functionName << endl;
  outf << "0;JMP" << endl;

  outf << "(RETURN" << codewriter_number << ")" << endl;
  codewriter_number++;
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
