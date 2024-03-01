#include <iostream>
#include <deque>
#include "ospath.h"
#include "stringExpand.h"

//system Header
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
using namespace std;

#define PROMPT          cout<<"junhyeong >> "
#define QEMUOPT         "make qemu"
#define SOURCE_CODE_DIR "소스코드" 
#define ASSIGN_DIR      "~/Assign"
#define XV6_CLONEDIR    "xv6_clone"


string curPath;
void        installer_0();
string      findAssigns_1();
string      unzipAssign_2(string s);
int         clon_xv6_3();
int         moveFile_4(string s);
int         qemu_5();
int         delete_tmpFile_6();

void        totalModule();

//signal Handler
static void sigHandlerInt(int signo);

//error Func
void __error(string s) {
    fprintf(stderr, "%s\n", s.c_str());
    exit(1); 
}

int main(void) {
    totalModule();
    exit(0);
}

void totalModule()
{
    installer_0();
    curPath = getcwd(NULL, 0);
    string paths = findAssigns_1();
    string dir_path = unzipAssign_2(paths);
    if (!clon_xv6_3())
        __error("clone_xv6 func error");
    if (!moveFile_4(dir_path))
        __error("moveFile func error");

    if (!qemu_5())
        __error("qemu func error");
    if (!delete_tmpFile_6())
        __error("delete_tmpFile Func error");
    return;
}

int         delete_tmpFile_6()
{
    path p(ASSIGN_DIR);
    deque<string> eraseQ = p.readDir();
    for (int i = 0 ; i < eraseQ.size() ; i++) {
        if (strstr(eraseQ.at(i).c_str(), "_tmp")) {
            cout<<"unlink --> "<<eraseQ.at(i)<<endl;
            system(string("rm -rf " + strip(eraseQ.at(i))).c_str());
        }
    }
    return true;
}
static void sigHandlerInt(int signo)
{
    fprintf(stderr, "Interrupt Occured : SINGINT\n");
    exit(0);
}

int         qemu_5()
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid != 0) 
        wait(NULL);
    else {
        signal(SIGINT, sigHandlerInt);
        string debug_opt;
        cin.ignore();
        cout << "Debug option (없으면 Enter) >> ";

        getline(cin, debug_opt);

        if (!debug_opt.compare(""))
            system(QEMUOPT);
        else
            system(string(QEMUOPT + string(" ") + strip(debug_opt)).c_str());
        return true;
    }
    return true;
}
int     moveFile_4(string s) {
    path p(s);
    if (p.getCurPath() == "")
        return false;
    
    string sourceDir = p.join({strip(p.getCurPath()), strip(SOURCE_CODE_DIR)});
    if (!p.isDir(sourceDir))
        return false;
    
    p.dirCopy(sourceDir, p.join({strip("~"), strip(XV6_CLONEDIR)}));
    return true;
}

int     clon_xv6_3() 
{
    path p("~");
    string cloneDir = p.join({strip(p.getCurPath()), strip(XV6_CLONEDIR)});
    
    //Not Directory --> Make
    if (!p.isDir(cloneDir)) {
        cout<<"mkdir : "<<cloneDir<<endl;
        if (mkdir(cloneDir.c_str(), 0755) < 0) {
            fprintf(stderr, "[clone_xv6_3] mkdir eroror : %s\n", cloneDir.c_str());
            return false;
        }
    }

    chdir(getenv("HOME"));
    cout<<cloneDir<<endl;
    system(string("rm -rf " + cloneDir + "/*").c_str());
    system("/bin/bash -c 'git init'");
    system(string("git clone https://github.com/mit-pdos/xv6-public.git").c_str());

    p.dirCopy("xv6-public", cloneDir);
    chdir(cloneDir.c_str()); //경로 변경
    return true;
}

void installer_0()
{
    string originStr = "sudo apt-get install ";
    string install_vec[] = {
        "zip", "openssl"
    };

    for (int i = 0 ; i < sizeof(install_vec) / sizeof(install_vec[0]) ; i++) {
        string lines = originStr + install_vec[i];
        system(lines.c_str());
    }
}

/**
 * Parameter s 로부터 얻은 파일 경로
 * @param s unzip할 파일 경로
 * @return tempDirectory
*/
string unzipAssign_2(string s)
{
    if (s == "")
        return "";

    path p(s);
    string momPath = p.motherDir(p.getCurPath());
    cout<<momPath<<endl;

    //tempDir : HHMMSS
    char tmpDirName[11] = {0,};
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(tmpDirName, "%02d%02d%02d_tmp", tm->tm_hour, tm->tm_min, tm->tm_sec);
    string tmpDir = p.join({strip(momPath), strip(tmpDirName)});

    //make dir 
    if (mkdir(tmpDir.c_str(), 0755) < 0) {
        fprintf(stderr, "make directory Error : %s\n", tmpDir.c_str());
        return "";
    }

    //unzip File
    char unzipLine[1024];
    sprintf(unzipLine, "unzip -O cp949 %s -d %s", s.c_str(), tmpDir.c_str());
    system(unzipLine); 
    return tmpDir;
}

string findAssigns_1()
{
    path p(ASSIGN_DIR);
    deque<string> assignQ = p.readDir();
    for (int i = 0 ; i < assignQ.size() ; i++) {
        if (p.isDir(assignQ[i])) continue;
        cout<<"["<< i <<"] :: "<<assignQ[i]<<endl;
    }

    int yourNum = -1;
    for (int atpt = 0 ; yourNum < 0 || yourNum > assignQ.size() ; atpt++) {
        if (atpt)
            cout<<"Wrong Value [MAXSIZE="<< assignQ.size() <<"]"<<endl;
        PROMPT;
        cin>>yourNum;
        cin.ignore(0);
    }
    return assignQ[yourNum] ;
}