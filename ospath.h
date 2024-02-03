#ifndef OSPATH_H
#define OSPATH_H
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "stringExpand.h"
using namespace std;
const char* pathToken = "/";

class path {
private:
    string curPath;

public:
    path (string s = getcwd(NULL, 0));
    ~path() {}    
    string getCurPath();   // 현재경로
    string fullPath(string s);  // s를 full 경로로 변경
    string join(deque<string> d); //deque 값으로 path 경로 합치기

    bool isFile(string s);      // 파일 존재 여부 확인
    bool isDir(string s);       // 파일이 디렉토리 인지 확인 
    bool fileCopy (string a, string b);

    deque<string> readDir(string s, bool all);
}; 

bool path::fileCopy (string a, string b) {

}


deque<string> path::readDir(string s = "", bool all = true)
{
    if (s == "")
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

                pq.push_back(join({curPath, string(dnt->d_name)}));
                cout<<join({curDirPath, string(dnt->d_name)})<<endl;
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
        if (!d.empty())
            retVal += pathToken;
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