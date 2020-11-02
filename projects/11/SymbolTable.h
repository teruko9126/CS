//
// Created by 木本舟 on 11/1/20.
//

#ifndef PROJECTS_SYMBOLTABLE_H
#define PROJECTS_SYMBOLTABLE_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

enum Kind{
  STATIC,
  FIELD,
  ARG,
  VAR
};

class Symboltable
{
public:
  Symboltable();
  ~Symboltable();
  void define(string name,string type,Kind kind);
  int VarCount(Kind kind);
  Kind kindOf(string name);
  string typeOf(string name);
  int indexOf(string name);

private:
};

#endif //PROJECTS_SYMBOLTABLE_H
