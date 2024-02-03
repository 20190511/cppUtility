//Made By Junhyeong(0511) || Linux C++ SystemCall Utility Project

#ifndef OSPATH_H
#define OSPATH_H
#include <iostream>
#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "stringExpand.h"
using namespace std;
const char* pathToken = "/";

class path {
private:
    string      curPath;
    const int   READBUF = 1024;

public:
    path (string s = getcwd(NULL, 0));
    ~path() {}    
    string getCurPath();            // 현재경로
    string fullPath(string s);      // s를 full 경로로 변경
    string join(deque<string> d);   //deque 값으로 path 경로 합치기
    string motherDir(string s);     //모체 디렉토리

    bool isFile(string s);      // 파일 존재 여부 확인
    bool isDir(string s);       // 파일이 디렉토리 인지 확인 
    bool mkdirs(string s);      // 복수 디렉토리 생성
    bool fileCopy (string a, string b); //파일 복사
    bool dirCopy (string a, string b); //디렉토리 복사

    deque<string> readDir(string s, bool all);
}; 

bool path::mkdirs(string s) {
    deque<string> sq = split(s, "/");

    string start = "/";
    while(!sq.empty()) {
        start += sq.front();
        sq.pop_front();
        if (!isFile(start))   {
            if (mkdir(start.c_str(), 0777) < 0) {
                fprintf(stderr, "{mkdirs} mkdir error : %s\n", start.c_str());
                return false; 
            }
        }
        if (!sq.empty()) 
            start += pathToken;
    }
    
    return true;
}


string path::motherDir(string s) {
    char p[1024] = {0,}, *ptr;
    strcpy(p, s.c_str());
    ptr = strrchr(p, pathToken[0]);
    if (ptr == NULL)
        return s;
    *ptr = 0;
    return string(p);
}

bool path::dirCopy (string a="", string b="../backup") {
    if (!a.length())
        a = curPath;
    a = fullPath(a), b = fullPath(b);

    if (!isDir(a)) {
        fprintf(stderr, "{dirCopy} %s is not directory!!\n", a.c_str());
        return false;
    }
    else if (!access(b.c_str(), F_OK) && !isDir(b)) {
        fprintf(stderr, "{dirCopy} %s is already existed (And not direcotry)!!\n", b.c_str());
        return false;
    }
    else if (access(b.c_str(), F_OK)) {
        cout<<"make directory --> "<<b<<endl;
        mkdir(b.c_str(), 0777);
    }

    deque<string> fq = readDir(a, true);
    while(!fq.empty()) {
        string curFullS = fq.front();
        string curS = string(curFullS).replace(0, a.length(), "");
        fq.pop_front();
        string nextS = join({b,curS});

        if (isDir(curFullS)) {
            if (isFile(nextS)) {
                if (isDir(nextS))
                    continue;
                else {
                    fprintf(stderr, "{dirCopy} %s can not be set directory\n", nextS.c_str());
                    continue;
                }
            }
            else {
                cout<<"make directory --> "<<nextS<<endl;
                if (mkdir(nextS.c_str(), 0777) < 0) {
                    fprintf(stderr, "mkdir error : %s\n", nextS.c_str());
                    continue;
                }
            }
        }
        else {
            fileCopy(curFullS, nextS);
        }
    }
    return true;
}

bool path::fileCopy (string a="", string b="../backup") {
    if (a == "")
        a = curPath;
    
    if (isDir(a)) {
        fprintf (stderr, "{fileCopy} %s is directory File\n", a.c_str());
        return false;
    }

    string md = motherDir(a);
    if (!mkdirs(md))  {
        fprintf(stderr, "mkdirs error : %s\n", md.c_str());
        return false;
    }
    
    cout<<a<<" --> "<<b<<endl;    
    int fd1, fd2, d;
    char buf[READBUF];
    if ((fd1 = open(a.c_str(), O_RDONLY)) < 0) {
        fprintf(stderr, "%s can not open\n", a.c_str());
        return false;
    }


    if ((fd2 = open(b.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0744)) < 0) {
        fprintf(stderr, "%s can not open\n", a.c_str());
        return false;
    }

    while((d = read(fd1, buf, READBUF)) > 0) {
        if (write(fd2, buf, d) != d) {
            fprintf(stderr, "{fileCopy} %s file Copy error\n", a.c_str());
            close(fd1);
            close(fd2);
            return false;
        }
    }

    close(fd1);
    close(fd2);
    return true;
}


deque<string> path::readDir(string s = "", bool all = true)
{
    if (!s.length())
        s = curPath;
    deque<string> pq, dirQueue({s});
    if (!isDir(curPath))
        return pq;

    while (!dirQueue.empty()) {
        string curDirPath = dirQueue.front();
        dirQueue.pop_front();

        DIR *dir = opendir(curDirPath.c_str());
        struct dirent *dnt;
        if (dir == NULL) {
            fprintf(stderr, "readdir error %s\n", curDirPath.c_str());
            break;
        }

        while ((dnt = readdir(dir)) != NULL) {
            if (strcmp(dnt->d_name, ".") && strcmp(dnt->d_name, "..")) {
                if (isDir(dnt->d_name) && all) {
                    dirQueue.push_back(join({curDirPath, string(dnt->d_name)}));
                }
                pq.push_back(join({curDirPath, string(dnt->d_name)}));
                //cout<<join({curDirPath, string(dnt->d_name)})<<endl;
            }
        }
    }
    return pq;
}
/**
 * @param deque<string> d 
 * @warning 사용법 : join({a.getCurPath(), "pkfg", "ss"})
 **/ 
string path::join(deque<string> d) 
{
    string retVal;
    while(!d.empty()) {
        retVal += d.front();
        d.pop_front();
        if (!d.empty()) {
            if (d.front().at(0) != pathToken[0])
                retVal += pathToken;
        }
        else
            break;
    }
    return retVal;
}

string path::fullPath(string s) {
    if (s.at(0) == '~') {
        s = getenv("HOME") + s.substr(1, s.length()-1);
    }
    else if (s.at(0) != pathToken[0]) {
       s = string(getcwd(NULL, 0)) + pathToken + s;
    }  

    cout<<s<<endl;
    deque<string> dq = split(s, pathToken), dd;
    while(!dq.empty()) {
        if (!dq.front().compare(".")) 
            continue;
        else if (!dq.front().compare(".."))  {
            if(!dd.empty())
                dd.pop_back();
        }
        else
            dd.push_back(dq.front());   
        dq.pop_front();
    }
 
    string s2 = pathToken;
    while(!dd.empty()) {
        s2 += dd.front();
        dd.pop_front();
        if (!dd.empty()) {
            s2 += pathToken;
        }
        else
            break;
    }   
    //cout<<"full path :: " + s2<<endl;
    return s2;
}

bool path::isFile(string s) {
    return !access(s.c_str(), F_OK);
}

bool path::isDir(string s) {
    if (!isFile(s)) {
        return false;
    }

    struct stat sb;
    if (stat(s.c_str(), &sb) < 0) {
        fprintf(stderr, "%s cannot be accessed statbuf\n", s.c_str());
        return false;
    }

    return S_ISDIR(sb.st_mode);
}

string path::getCurPath() {
        return curPath;
}

path::path(string s) { 
    s = fullPath(s);

    if (!isFile(s)) {
        cout<<s<< " is not existed"<<endl;
        path::curPath = "";
        path::~path();
        return;
    }
    struct stat st;
    path::curPath = s;
}



#endif