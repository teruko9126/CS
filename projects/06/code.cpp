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
    else
    {
      temp += data[i];
    }
    i++;
  }
  order.second = temp;
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

  map<string, int> symbol;
  symbol["SP"] = 0;
  symbol["LCL"] = 1;
  symbol["ARG"] = 2;
  symbol["THIS"] = 3;
  symbol["THAT"] = 4;
  symbol["R0"] = 0;
  symbol["R1"] = 1;
  symbol["R2"] = 2;
  symbol["R3"] = 3;
  symbol["R4"] = 4;
  symbol["R5"] = 5;
  symbol["R6"] = 6;
  symbol["R7"] = 7;
  symbol["R8"] = 8;
  symbol["R9"] = 9;
  symbol["R10"] = 10;
  symbol["R11"] = 11;
  symbol["R12"] = 12;
  symbol["R13"] = 13;
  symbol["R14"] = 14;
  symbol["R15"] = 15;
  symbol["SCREEN"] = 16384;
  symbol["KBD"] = 24576;
  string inname;
  inname = argv[1];

  int phase = 1;
  while (phase <= 2)
  {

    fin.open(inname);
    if (fin.fail())
    {
      cout << "opening file failed" << endl;
      return 0;
    }

    int dot = inname.find(".");

    string outname = inname.substr(0, dot);

    outname = outname + ".hack";

    ofstream writing_file;
    writing_file.open(outname, ios::out);

    //if文で//以降の部分は全部省略しよう
    //もし何もdataがないならそのまま次の文章読み込みに入る
    string data;
    int counter = 0;
    int newvar = 16;
    while (getline(fin, data))
    {
      string binarydata;
      int i = 0;
      //空白文字を消すようにしている
      while (i < data.size())
      {
        if (isspace(data[i]))
        {
          data.erase(data.begin() + i);
        }
        else
        {
          i++;
        }
      }
      string::size_type pos = data.find("//");
      if (pos != string::npos)
      {
        data.erase(data.begin() + pos, data.end());
      }
      //cout << data << endl;
      if (data[0] == '@')
      {
        //変数シンボルはメモリアドレス16番以降におく必要あり
        writing_file << "0";
        data = data.substr(1);
        int num = 0;
        if (isdigit(data[0]))
        {
          num = atoi(data.c_str());
          writing_file << bitset<15>(num) << endl;
        }
        else if (phase == 2)
        {
          //variable(飛ぶためのテーブル)に入っていない、かつ決められたシンボルでもないものが変数として選ばれる
          //新しく割り当てたものに関しては16番以降のアドレスを与える必要あり

          auto it = symbol.find(data);
          if (it == symbol.end())
          {
            symbol[data] = newvar;
            num = newvar;
            newvar++;
          }
          else
          {
            num = symbol[data];
          }
          //わざわざvariableテーブルを作る必要あるか？？？？
          //symbolテーブルに入っていないなら新しく値を入れる仕組みにしたらいい
          //決められた値については対応表を作る必要あり
          writing_file << bitset<15>(num) << endl;
        }
        counter++;
      }
      else if (data[0] == '(')
      {
        data = data.substr(1, data.size() - 2);
        symbol[data] = counter;
      }
      else if (data.size() > 1)
      {
        string comp = "0000000";
        string dest = "000";
        string jump = "000";

        writing_file << "111";
        //destとcompで命令が構築されている
        if (data.find('=') != string::npos)
        {
          pair<string, string> destcomp;
          destcomp = orderA(data);
          dest = destmap[destcomp.first];
          comp = compmap[destcomp.second];
          writing_file << comp << dest << jump << endl;
        }
        else if (data.find(';') != string::npos)
        {
          pair<string, string> compjump;
          compjump = orderA(data);
          comp = compmap[compjump.first];
          jump = jumpmap[compjump.second];
          writing_file << comp << dest << jump << endl;
        }
        counter++;
      }
    }
    phase++;
    fin.close();
    writing_file.close();
  }
}

//dest=comp;jumpなのでうまくそれぞれの部分だけが入っているように分割してあげたい
//=は必ず入っているわけではない(destがないなら省略可能であるため)
//;も同じく必ず入っているわけではない
//最初に値を代入するようにしてもそれがdestかcompかわからないのが問題点
//=が入っているか;が入っているかで命令の区別をしたらいい
