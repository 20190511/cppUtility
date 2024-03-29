//Made By Junhyeong(0511) : Linux Shell Scriptor Reader Project (24.02.03 ~ 24.02.04)
/**
 * Debug 및 수정예정 사항
 * 
 * 예정
 *  1. chdir 대신 cd를 자동으로 chdir로 인식
 *  2. 실험 Experience >> CoTTa, xv6 Virtual OS Project
 */

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
#define MOD_CHDIR       4
#define MOD_RECHDIR     5

#define SMODE_FILECHECK   "[filecheck]"


const char* modeList[6] = {
    "install:",
    "run:",
    "delete:",
    "copy:",
    "chdir:",
    "reChdir:"
};

deque<pair<int, string>> cmdQ;
deque<string>   pathStack;

char* strToChar(string s);
void transformer();

//Special 명령어 실행
void doSpecialCmd(int mode) {
    if (mode == -1)
        return;
    
}


// 패키지 설치 관련
void installer(string s);
void installer(deque<string> q);

// systemFunc
void system(string s);

//deleteSystem
void deleteFiles(string s);

//change Directory
void chdirs(string s);

int main(void)
{
    transformer(); 
    int curSet = -1;
    while (!cmdQ.empty()) {
        pair<int, string> element_cmd = cmdQ.front();
        cmdQ.pop_front();

        // White Space 제거
        if (!strip(element_cmd.second).length())
            continue;

        switch (element_cmd.first) {
        case MOD_INSTALL:
            installer(element_cmd.second);
            break;
        case MOD_RUN:
            system(element_cmd.second);
            break;
        case MOD_DELETE:
            deleteFiles(element_cmd.second);
            break;
        case MOD_COPY:
        {
            deque<string> cpyQ = splitString(string(element_cmd.second), "->", 2);
            if (cpyQ.empty())
            {
                fprintf(stderr, "Copy Cmd is Error : %s\n", element_cmd.second.c_str());
                break;
            }
            string prevPath = strip(cpyQ.at(0)), nextPath = strip(cpyQ.at(1));
            path p(prevPath);
            if (!p.getCurPath().length()) //Not file prev_path
                break;
            p.dirCopy(prevPath, nextPath);
            break;

        }
        case MOD_CHDIR:
            chdirs(element_cmd.second);
            break;
        case MOD_RECHDIR:
        {
            if (pathStack.empty())
                break;

            string retAddr;
            if (element_cmd.second.compare("init")) {
                retAddr = string(pathStack.front());
                pathStack.clear();
            }
            else {
                retAddr = string(pathStack.back()); 
                pathStack.pop_back();
            }
                
            
            if (chdir(retAddr.c_str()) < 0) {
                fprintf(stderr, "chdir error : ");
                break;
            }
            cout<<"curPath : "<<getcwd(NULL, 0)<<endl;
            break;
        }
        default:
            break;
        }
        curSet = element_cmd.first;

    }

    exit(0);
}

void chdirs(string s) {
    path p(s);
    if (!p.getCurPath().length())
        return;

    pathStack.push_back(getcwd(NULL, 0));
    if (chdir(p.getCurPath().c_str()) < 0) {
         fprintf(stderr, "chdir error : %s\n", p.getCurPath().c_str());
         pathStack.pop_back();
    }
    cout<<"curPath : "<<getcwd(NULL,0)<<endl;
}

void deleteFiles(string s)
{
    path p(s);
    if (!p.getCurPath().length())
        return; 
    p.fileDelete();
}

//@OverLoading -> int system (const char* _cmd); 
/**
 * Linux Standard Header : Deafault Shell Cshell
 *  >> system() Library 는 기본적으로 subRoutine (Procedure) 를 가지지 않기 때문에
 *  fork() + execl() 로 cpp에서 구현해야 conda activate  를 구현가능함
 *      --> Python에서 subprogram을 지원하기 때문에 파이썬으로 자동연결 예정 (24.02.04 : (jh0511))
 **/
void system(string s)
{
    system(s.c_str());
}

void transformer() {
    path p(".");
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
        if (cnt) cout<<"\nRetry, Your Key is Wrong"<<endl;

        cout << "===== Select Script File =====" << endl;
        for (int idx = 0; idx < scriptQ.size(); idx++) {
            cout << "[" << idx << "] : " << scriptQ.at(idx) << endl;
        }

        PROMPT;
        cin >> select;
        cin.ignore();
    }

    // Read The Script
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
        
        // 빈칸 삭제
        for(; *line == ' ' ; line++);
        
        // 스크립트 적용 및 queue에 삽입
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

        //Special Command Cheker (24.02.05 : Junhyeong)
        // --> Script 에서 요구하는 Special 명령어 검사
        int isFile = false;
        if (strstr(line, SMODE_FILECHECK)) {
            deque<string> tmpString = splitString(line, SMODE_FILECHECK); 
            path p(strip(tmpString.back()));
            if (p.isFile(p.getCurPath())) 
                isFile = true;
            else
                strcpy(line, strip(tmpString.at(0)).c_str());
        }

        if (curMod == -1)    
            cout<<"wrong mode"<<endl;
        else if (isFile)
            cout<<"your file is already existed {does not execute : "<<line<<":}"<<endl;
        else
            cmdQ.push_back(pair<int,string>(curMod, line));
        memset(line, 0, 1024);
    }

    fclose(fp);
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