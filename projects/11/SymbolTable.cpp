//
// Created by 木本舟 on 11/1/20.
//

#include "SymbolTable.h"

bool isKey(map<string, ST> & scope, string key)
{
  return !(scope.find(key) == scope.end());
}

Symboltable::Symboltable() {
  class_scope.clear();
  subroutine_scope.clear();
  static_num = 0;
  field_num = 0;
  argument_num = 0;
  var_num = 0;
}

void Symboltable::startSubroutine() {
  subroutine_scope.clear();
  argument_num = 0;
  var_num = 0;
}

void Symboltable::define(string name, string type, symboltable::Kind kind) {
  if(kind == symboltable::STATIC){
    class_scope[name].type = type;
    class_scope[name].kind = kind;
    class_scope[name].index = static_num;
    static_num++;
  }
  else if(kind == symboltable::FIELD){
    class_scope[name].type = type;
    class_scope[name].kind = kind;
    class_scope[name].index = field_num;
    field_num++;
  }
  else if(kind == symboltable::ARG){
    subroutine_scope[name].type = type;
    subroutine_scope[name].kind = kind;
    subroutine_scope[name].index = argument_num;
    argument_num++;
  }
  else if(kind == symboltable::VAR){
    subroutine_scope[name].type = type;
    subroutine_scope[name].kind = kind;
    subroutine_scope[name].index = var_num;
    var_num++;
  }
}

int Symboltable::VarCount(symboltable::Kind kind) {
  switch (kind) {
    case symboltable::STATIC:
      return static_num;
    case symboltable::FIELD:
      return field_num;
    case symboltable::ARG:
      return argument_num;
    case symboltable::VAR:
      return var_num;
    default:
      return -1;
  }
}

symboltable::Kind Symboltable::kindOf(string name) {
  if(isKey(class_scope,name))
    return class_scope[name].kind;
  else if(isKey(subroutine_scope,name))
    return subroutine_scope[name].kind;
  else
    return symboltable::NONE;
}

string Symboltable::typeOf(string name) {
  if(isKey(class_scope,name))
    return class_scope[name].type;
  else if(isKey(subroutine_scope,name))
    return subroutine_scope[name].type;
  else
    return "";
}

int Symboltable::indexOf(string name) {
  if(isKey(class_scope,name))
    return class_scope[name].index;
  else if(isKey(subroutine_scope,name))
    return subroutine_scope[name].index;
  else
    return -1;
}
