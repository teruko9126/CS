#include <iostream>
#include <fstream>
#include <bitset>
#include <ctype.h>
#include <map>
#include <utility> //pair
#include <string>
using namespace std;

#define bufsize 3000

//何が欲しいか
//=と;とspaceを無視して、左辺と右辺を返してくれる関数が欲しい
//引数としてそのまま一行を持ってきたい

pair<string, string> orderA(string data)
{
  pair<string, string> order;
  int i = 0;
  string temp = ""; //とりあえず受け皿として用意している(=とか;がくれば対応したところに入れてリセット)
  while (i < data.size())
  {
    if (data[i] == '=' || data[i] == ';')
    {
      order.first = temp;
      temp = "";
    }
    else if (!isspace(data[i]))
    {
      temp += data[i];
    }
    i++;
  }
  order.second = temp;
  //cout << order.first << endl;
  //cout << order.second << endl;
  return order;
  //コメントを無視する関数を入れていないのでそこでバグる危険性あり
}

int main(int argc, char *argv[])
{

  map<string, string> compmap;

  compmap["0"] = "0101010";
  compmap["1"] = "0111111";
  compmap["-1"] = "0111010";
  compmap["D"] = "0001100";
  compmap["A"] = "0110000";
  compmap["!D"] = "0001101";
  compmap["!A"] = "0110001";
  compmap["-D"] = "0001111";
  compmap["-A"] = "0110011";
  compmap["D+1"] = "0011111";
  compmap["A+1"] = "0110111";
  compmap["D-1"] = "0001110";
  compmap["A-1"] = "0110010";
  compmap["D+A"] = "0000010";
  compmap["D-A"] = "0010011";
  compmap["A-D"] = "0000111";
  compmap["D&A"] = "0000000";
  compmap["D|A"] = "0010101";
  compmap["M"] = "1110000";
  compmap["!M"] = "1110001";
  compmap["-M"] = "1110011";
  compmap["M+1"] = "1110111";
  compmap["M-1"] = "1110010";
  compmap["D+M"] = "1000010";
  compmap["D-M"] = "1010011";
  compmap["M-D"] = "1000111";
  compmap["D&M"] = "1000000";
  compmap["D|M"] = "1010101";

  map<string, string> jumpmap;

  jumpmap["null"] = "000";
  jumpmap["JGT"] = "001";
  jumpmap["JEQ"] = "010";
  jumpmap["JGE"] = "011";
  jumpmap["JLT"] = "100";
  jumpmap["JNE"] = "101";
  jumpmap["JLE"] = "110";
  jumpmap["JMP"] = "111";

  map<string, string> destmap;

  destmap["null"] = "000";
  destmap["M"] = "001";
  destmap["D"] = "010";
  destmap["MD"] = "011";
  destmap["A"] = "100";
  destmap["AM"] = "101";
  destmap["AD"] = "110";
  destmap["AMD"] = "111";
  ifstream fin;
  ofstream fout;

  string inname;
  inname = argv[1];
  //cout << inname << endl;

  fin.open(inname);
  if (fin.fail())
  {
    cout << "opening file failed" << endl;
    return 0;
  }

  int dot = inname.find(".");
  //cout << dot << endl;

  string outname = inname.substr(0, dot);

  outname = outname + ".hack";
  //cout << outname << endl;

  ofstream writing_file;
  writing_file.open(outname, ios::out);

  //if文で//以降の部分は全部省略しよう
  //もし何もdataがないならそのまま次の文章読み込みに入る
  string data;
  int i = 0;
  while (getline(fin, data))
  {
    string binarydata;
    string::size_type pos = data.find("//");
    if (pos != string::npos)
    {
      data.erase(data.begin() + pos, data.end());
    }
    //cout << data << endl;
    if (data[0] == '@')
    {
      writing_file << "0";
      string strnum = data.substr(1, data.size());
      int num = atoi(strnum.c_str());
      writing_file << bitset<15>(num) << endl;
      //cout << bitset<15>(num) << endl;
    }
    else if (data.size() > 1)
    {
      string comp = "0000000";
      string dest = "000";
      string jump = "000";

      //cout << "C" << endl;
      writing_file << "111";
      //destとcompで命令が構築されている
      if (data.find('=') != string::npos)
      {
        pair<string, string> destcomp;
        destcomp = orderA(data);
        cout << "first" << destcomp.first << endl;
        cout << "second" << destcomp.second << endl;
        dest = destmap[destcomp.first];
        comp = compmap[destcomp.second];
        cout << dest << endl;
        cout << comp << endl;
        writing_file << comp << dest << jump << endl;
      }
      else if (data.find(';') != string::npos)
      {
        pair<string, string> compjump;
        compjump = orderA(data);
        //cout << compjump.first << endl;
        //cout << compjump.second << endl;
        comp = compmap[compjump.first];
        jump = jumpmap[compjump.second];
        writing_file << comp << dest << jump << endl;
      }
    }
    i++;
  }
}

//dest=comp;jumpなのでうまくそれぞれの部分だけが入っているように分割してあげたい
//=は必ず入っているわけではない(destがないなら省略可能であるため)
//;も同じく必ず入っているわけではない
//最初に値を代入するようにしてもそれがdestかcompかわからないのが問題点
//=が入っているか;が入っているかで命令の区別をしたらいい
