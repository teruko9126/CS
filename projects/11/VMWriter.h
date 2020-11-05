//
// Created by 木本舟 on 11/1/20.
//

#ifndef PROJECTS_VMWRITER_H
#define PROJECTS_VMWRITER_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

namespace vmwriter{
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


  string segment2string(Segment segment) {
    if (segment == CONST) return "constant ";
    if (segment == ARG) return "argument ";
    if (segment == LOCAL) return "local ";
    if (segment == STATIC) return "static ";
    if (segment == THIS) return "this ";
    if (segment == THAT) return "that ";
    if (segment == POINTER) return "pointer ";
    if (segment == TEMP) return "temp ";
    else return "error segment2string";
  }

  string command2string(Command command) {
    if (command == ADD) return "add";
    if (command == SUB) return "sub";
    if (command == NEG) return "neg";
    if (command == EQ) return "eq";
    if (command == GT) return "gt";
    if (command == LT) return "lt";
    if (command == AND) return "and";
    if (command == OR) return "or";
    if (command == NOT) return "not";
    else return "error command2string";
  }
}

class VMWriter
{
public:
  VMWriter(string filename);
  ~VMWriter();
  void writePush(vmwriter::Segment segment,int index);
  void writePop(vmwriter::Segment segment,int index);
  void writeArithmetic(vmwriter::Command command);
  void writeLabel(string label);
  void writeGoto(string label);
  void writeIf(string label);
  void writeCall(string label,int nArgs);
  void writeFunction(string name,int nLocals);
  void writeReturn(void);
  void close(void);

  string segment2string(vmwriter::Segment segment);
  string command2string(vmwriter::Command command);
  string vmwriter_filename;

private:
  ofstream outf;
};

#endif //PROJECTS_VMWRITER_H
