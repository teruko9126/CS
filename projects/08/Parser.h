#ifndef _PERSER_
//このhファイルに関しては一回だけ読み込めばいいため、ifndefで囲んでいる
#define _PERSER_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum VMcommands
{
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

class Parser
{
public:
    Parser(string filename);
    ~Parser(void);
    bool hasMoreCommands(void);
    void advance(void);
    VMcommands commandType(void);
    string arg1(void);
    int arg2(void);
    string Getparserstring(void);

private:
    ifstream inf;
    int parser_lastslash;
    string parser_filename;
    string parser_currentcommand;
};

#endif