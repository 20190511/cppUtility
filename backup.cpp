#include <iostream>
#include <string>
#include <cstdlib>
#include "backup.hpp"

//System Header
#include <wait.h>

using namespace std;

#define BACKUP_FLAG         0b1
#define REMOVE_FLAG         0b10
#define RECOVER_FLAG        0b100
#define LS_FLAG             0b1000
#define HELP_FLAG           0b10000
#define EXIT_FLAG           0b100000
#define VIM_FLAG            0b11000000

#define RECOVER_NEW         0x80000000
#define REMOVE_CLEAR        0x40000000

/**
 * 0b0          무옵션
 * 0b1          backup
 * 0b10         remove 
 * 0b100        remove -c
 * 0b1000       recover (default)
 * 0b10000      recover -n 
 * 0b100000     ls
 * 0b1000000    help
 * 0b10000000   exit
*/
string subOption = "";
unsigned int optFlag = 0x0;
int optCnt = 0;
char** optVector = NULL;
extern   int optind;

const string cmdList[] = {
    "backup", "remove", "recover", "ls", "help", "exit", "vi", "vim"
};
void optFunc();
void optClear();
bool optProcess();

void backupSystem();
int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <md5 | sha1>\n", argv[0]);
        exit(1);
    }

    if (!strcmp(argv[1], "md5"))
        hash_mode = 0;
    else if (!strcmp(argv[1], "sha1"))
        hash_mode = 1;
    else {
        fprintf(stderr, "Wrong file Checker Hash : Try Again <md5 | sha1>\n");
        exit(1);
    }

    hyperSetup();
    while(true) {

        optFunc();
        if (!optProcess())
            break;
        optClear();
    }
    //b_remove("/home/junhyeong/success", false);
    //backup("/home/junhyeong/success");
    //recover("/home/junhyeong/success", "") ;
    exit(0);
}

void backupSystem() {
    cout<<"Your Path : "<<optVector[1]<<endl;
    if (optFlag & BACKUP_FLAG)  {
        //backup();
    }
    else if (optFlag & REMOVE_FLAG) {
        //b_remove(,optFlag & REMOVE_CLEAR);
    }
    else if (optFlag & RECOVER_FLAG)  {
        //recover(,);
    }
}
bool optProcess()
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        return true;
    }
    else if (pid > 0) {
        wait(NULL);
        if (optFlag & EXIT_FLAG)
            return false;
        return true;
    }


    if (optFlag & (BACKUP_FLAG | REMOVE_FLAG | RECOVER_FLAG)) {
        backupSystem();
    }
    else if (optFlag & LS_FLAG) {
        execv("/bin/ls", optVector);
    }
    else if (optFlag & HELP_FLAG) {
        help();
    }
    else if (optFlag & VIM_FLAG) {
        execv("/bin/vim", optVector);
    }
    else if (!(optFlag & EXIT_FLAG)) {
        help();
    }

    if (pid == 0)
        exit(0);
    return true;
}

void optClear() {
    optFlag = 0;
    subOption = "";
    //동적할당 해제
    for (int i = 0 ; i < optCnt ; i++)    
        delete[] optVector[i];
    delete[] optVector;
    optVector = NULL;
    optCnt = 0;
}
void optFunc()
{
    int c;
    string line = "";

    while (!line.compare("")) {
        PROMPT;
        getline(cin, line);
    }
    deque<string> lineVec = split(line, " ");

    string cmd = lineVec.front();
    for (int i = 0 ; i < sizeof(cmdList) / sizeof(cmdList[0]) ; i++) 
        if (!cmd.compare(cmdList[i])) 
            optFlag |= 1<<i;

    // 추가 Option 처리
    optVector = new char*[(int)lineVec.size()+1]; //optCnt, optVector 설정
    while (!lineVec.empty()) {
        string tmp = lineVec.front();
        lineVec.pop_front();
        optVector[optCnt] = new char[tmp.length() + 1];
        strcpy(optVector[optCnt++], tmp.c_str());
    }
    optVector[optCnt] = NULL;

    if ((optFlag & (VIM_FLAG | LS_FLAG | EXIT_FLAG | HELP_FLAG)))
        return;
    
    //getopt 처리
    while((c = getopt(optCnt, optVector, "nc")) != -1) {
        switch(c) {
        case 'n':
            optFlag |= RECOVER_NEW; 
            subOption = optVector[optind];
            break;
        case 'c':
            optFlag |= REMOVE_CLEAR;
            break; 
        case '?':
            break;
        }
    }

}