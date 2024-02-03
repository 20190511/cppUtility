#include <iostream>
#include <map>
#include <queue>
#include <cstdio>
#include <deque>
#include <cstring>

//My HeaderFile (Junhyeong)
#include "ospath.h"
#include "stringExpand.h"

//Linux System Header
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>
using namespace std;

#define PROMPT          cout<<"junhyeong >> "
#define EXECUTE_FILE    ".jh"

#define MOD_INSTALL     0
#define MOD_RUN         1
#define MOD_DELETE      2
#define MOD_COPY        3

const char* modeList[4] = {
    "install:",
    "run:",
    "delete:",
    "copy:"
};


deque<string> installQ;
deque<string> runQ;
deque<string> deleteQ;
deque<pair<string,string>> copyQ;

char* strToChar(string s);
void transformer();

// 패키지 설치 관련
void installer(string s);
void installer(deque<string> q);

int main(void)
{
    pid_t pid;
    transformer();

    exit(0);
}

void transformer() {
    path p;
    deque<string> fileQ = p.readDir(), scriptQ;
    while(!fileQ.empty()) {
        string f = fileQ.front();
        fileQ.pop_front();
        if(strstr(f.c_str(), EXECUTE_FILE)) {
            scriptQ.push_back(f);
        }
    }
    
    //스크립트 선택
    int select = -1;
    for (int cnt = 0 ; select < 0 || select >= scriptQ.size() ; cnt++) {
        if (cnt)
            cout<<"\nRetry, Your Key is Wrong"<<endl;
        cout << "===== Select Script File =====" << endl;
        for (int idx = 0; idx < scriptQ.size(); idx++) {
            cout << "[" << idx << "] : " << scriptQ.at(idx) << endl;
        }
        PROMPT;
        cin >> select;
        cin.ignore();
    }

    string scriptFname = string(scriptQ.at(select));
    FILE* fp = fopen(scriptFname.c_str(), "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen error for %s\n", scriptFname.c_str());
        return;
    }
 
    char lineOrigin[1024] = {0,}, *line;
    int curMod = -1;
    while(!feof(fp)) {
        line = lineOrigin;
        fgets(line, 1024, fp);
        if (line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0';

        // 주석 삭제
        char* ptr = strstr(line, "#");
        if (ptr != NULL)  *ptr = 0;

        if (!strlen(line))
            continue;
        
        //빈칸 삭제
        for(; *line == ' ' ; line++);
        

        int chMod = false;
        for (int mod = 0 ; mod < sizeof(modeList) / sizeof(modeList[0]) ; mod++) {
            if (!strncmp(modeList[mod], line, strlen(modeList[mod]))) {
                curMod = mod; 
                chMod = true;
                break;
            }
        }
        if (chMod) {
            chMod = false;
            memset(line, 0, 1024);
            continue;
        }

        if (curMod == -1)   {
            cout<<"wrong mode"<<endl;
        }
        else {
            
            switch(curMod) {
            case MOD_INSTALL:
                installQ.push_back(line); break;
            case MOD_RUN:
                runQ.push_back(line); break;
            case MOD_DELETE:
                deleteQ.push_back(line); break;
            case MOD_COPY:
                deque<string> cpyQ = splitString(string(line), "->", 2);
                if (cpyQ.empty()) {
                    fprintf(stderr, "Copy Cmd is Error : %s\n", line);
                    break; 
                }
                copyQ.push_back(pair<string,string>(strip(cpyQ.at(0)), strip(cpyQ.at(1)))); 
                break;
            }
            
        }
        memset(line, 0, 1024);
        //installer(line);
    }

    fclose(fp);
    /*
    FILE* fp = fopen(INSTALLFILE, "r+");
    if (fp == NULL) {
        fprintf(stderr, "fopen error for %s\n", INSTALLFILE);
        return;
    }
    */

    //installer("yum");
}
    


void installer (deque<string> q) {
    while (!q.empty()) {
        installer(q.front());
        q.pop_front();
    }
}

void installer(string s) {
    const char* originStr = "sudo apt-get install";
    char line[1024];
    sprintf(line, "%s %s", originStr, s.c_str());
    cout<<line<<endl;
    system((const char*) line);
}