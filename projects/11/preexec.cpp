#include "preexec.h"

using namespace std;

preexec::preexec(string filename) {
  preexec_filename = filename;
  inf.open(preexec_filename.c_str());
}

preexec::~preexec() {
  if (inf.is_open())
    inf.close();
}

string preexec::getstring() {
  return preexec_current;
}

bool preexec::hasmorestring() {
  return !inf.eof();
}

void preexec::advance() {
  getline(inf,preexec_current);
}

void preexec::findsymbol() {
  int num = 0;
  int find1 = preexec_current.find("//");
  if(find1 != -1){
    preexec_current = preexec_current.substr(0,find1);
  }

  if(preexec_current.find("/**") != -1){
    while(preexec_current.find("*/") == -1){
      advance();
    }
    advance();
  }
  for (int i = 0; i < preexec_current.size(); i++) {
    if (preexec_current[i] == '{' || preexec_current[i] == '}' || preexec_current[i] == '(' ||
        preexec_current[i] == ')' || preexec_current[i] == '[' || preexec_current[i] == ']' ||
        preexec_current[i] == '.' || preexec_current[i] == ',' || preexec_current[i] == ';' ||
        preexec_current[i] == '+' || preexec_current[i] == '-' || preexec_current[i] == '*' ||
        preexec_current[i] == '/' || preexec_current[i] == '&' || preexec_current[i] == '|' ||
        preexec_current[i] == '<' || preexec_current[i] == '>' || preexec_current[i] == '=' ||
        preexec_current[i] == '~' || preexec_current[i] == '\"') {
      preexec_current = preexec_current.substr(0,i) + " " + preexec_current.substr(i,1) + " " + preexec_current.substr(i+1);
      i++;
    }
  }
}
