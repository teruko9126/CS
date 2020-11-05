//
// Created by 木本舟 on 11/1/20.
//

#ifndef PROJECTS_SYMBOLTABLE_H
#define PROJECTS_SYMBOLTABLE_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

namespace symboltable {
  enum Kind {
    STATIC,
    FIELD,
    ARG,
    VAR,
    NONE
  };
}

struct ST {
  symboltable::Kind kind;
  string type;
  int index;
};

class Symboltable {
public:
  Symboltable();

  void startSubroutine(void);

  void define(string name, string type, symboltable::Kind kind);

  int VarCount(symboltable::Kind kind);

  symboltable::Kind kindOf(string name);

  string typeOf(string name);

  int indexOf(string name);

private:
  map <string, ST> class_scope;
  map <string, ST> subroutine_scope;
  int static_num;
  int field_num;
  int argument_num;
  int var_num;
};

#endif //PROJECTS_SYMBOLTABLE_H
