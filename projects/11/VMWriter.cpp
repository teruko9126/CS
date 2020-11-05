#include "VMWriter.h"

VMWriter::VMWriter(string filename) {
  int dot = filename.find(".");
  vmwriter_filename = filename.substr(0,dot) + ".vm";
  outf.open(vmwriter_filename);
}

VMWriter::~VMWriter() {
  if (outf.is_open()) outf.close();
}

void VMWriter::writePush(vmwriter::Segment segment, int index) {
  outf << "push " << vmwriter::segment2string(segment) << index << endl;
}

void VMWriter::writePop(vmwriter::Segment segment, int index) {
  outf << "pop " << vmwriter::segment2string(segment) << index << endl;
}

void VMWriter::writeArithmetic(vmwriter::Command command) {
  outf << vmwriter::command2string(command) << endl;
}

void VMWriter::writeLabel(string label) {
  outf << "label " << label << endl;
}

void VMWriter::writeGoto(string label) {
  outf << "goto " << label << endl;
}

void VMWriter::writeIf(string label) {
  outf << "if-goto " << label << endl;
}

void VMWriter::writeCall(string label, int nArgs) {
  outf << "call " << label << " " << nArgs << endl;
}

void VMWriter::writeFunction(string name, int nLocals) {
  outf << "function " << name << " " << nLocals << endl;
}

void VMWriter::writeReturn() {
  outf << "return" << endl;
}
