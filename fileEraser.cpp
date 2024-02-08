#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <deque>
#include "ospath.h"
using namespace std;
char* curPath;


void eraseDirExecuteFile(const char *yourFile)
{
    path p(yourFile);
    deque<string> dirQ = p.readDir(p.getCurPath(), false);
    while (!dirQ.empty())
    {
        string curFile = dirQ.back();
        dirQ.pop_back();
        if (!strcmp(curPath, curFile.c_str()) || p.isDir(curFile))
            continue;
        else if (!strstr(curFile.c_str(), ".") && p.isExecute(curFile.c_str()))
        {
            cout << "delete your exeCute file -- > " << curFile.c_str() << endl;
            if (unlink(curFile.c_str()) < 0)
            {
                fprintf(stderr, "ulink error : %s\n", curFile.c_str());
                continue;
            }
        }
    }
}
int main(int argc , char* argv[])
{
    curPath = argv[0];
    if (argc < 2) {
        eraseDirExecuteFile(getcwd(NULL, 0));
    }
    
    for (int i = 1 ; i < argc ; i++) {
        eraseDirExecuteFile(*(argv+i));
    }
    exit(0);
}