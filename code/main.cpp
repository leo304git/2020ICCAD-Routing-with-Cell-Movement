#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"

using namespace std;

GgridMgr* ggridParser=new GgridMgr();


int main(int argc,char **argv)
{

    if(!ggridParser->readFile(argv[1])){cerr<<"error reading file?"<<endl; return 0;}
    ggridParser->placement(ggridParser->getCellInsts());

    //new
    ggridParser->routing();

    ggridParser->write_file();


    cout << "good!" << endl;

    return 0;
}
