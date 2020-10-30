#include "CompilationEngine.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

//TODO　tokenizerの初期設定が必要

CompilationEngine::CompilationEngine(string filename){
  outf.open(filename.c_str());
}

void CompilationEngine::compileClass() {
  outf << "<class>" << endl;
  tokenizer1.advance();
  assert(tokenizer1.keyWord() == CLASS);
  outf << "<keyword> class </keyword>" << endl;
  tokenizer1.advence();
  assert(tokenizer1.tokenType() == IDENTIFIER);
  outf << "<identifier>"
}






