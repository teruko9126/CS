#ifndef PROJECTS_PREEXEC_H
#define PROJECTS_PREEXEC_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class preexec{
public:
  preexec(string filename);
  ~preexec();
  bool hasmorestring();
  void advance();
  string getstring();
  void findsymbol(void);
  string preexec_current;

private:
  string preexec_filename;
  ifstream inf;
  ofstream outf;
};

#endif //PROJECTS_PREEXEC_H
