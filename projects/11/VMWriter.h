//
// Created by 木本舟 on 11/1/20.
//

#ifndef PROJECTS_VMWRITER_H
#define PROJECTS_VMWRITER_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

enum Segment{
  CONST,
  ARG,
  LOCAL,
  STATIC,
  THIS,
  THAT,
  POINTER,
  TEMP
};

enum Command{
  ADD,
  SUB,
  NEG,
  EQ,
  GT,
  LT,
  AND,
  OR,
  NOT
};

class VMWriter
{
public:
  VMWriter();
  ~VMWriter();
  void writePush(Segment segment,int index);
  void WritePop(Segment segment,int index);
  void WriteArithmetic(Command command);
  void WriteLabel(string label);
  void WriteGoto(string label);
  void WriteIf(string label);
  void WriteCall(string label,int nArgs);
  void WriteFunction(string name,int nLocals);
  void WriteReturn(void);
  void close(void);

private:
};

#endif //PROJECTS_VMWRITER_H
