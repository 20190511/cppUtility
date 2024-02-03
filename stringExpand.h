#ifndef STRINGEXPAND_H
#define STRINGEXPAND_H
#include <iostream>
#include <cstring>
#include <deque>

std::deque<std::string> split(std::string str, const char* tok, size_t sz = 0)
{ 
    char line[1024] = {0,}, *curPtr = line;
    strcpy(line, str.c_str());
    std::deque<std::string> dq;

    for (char* ptr = line ; *ptr != '\0' ; ptr++) { 
        for (const char* t = tok ; *t != '\0' ; t++)  {
            if (*ptr == *t) {
                *ptr = '\0';
                if (strlen(curPtr) != 0) {
                    dq.push_back(std::string(curPtr));
                    if (sz && sz <= dq.size())
                        return dq;
                }
                curPtr = ptr+1;
            }
        }
    }
    if (strlen(curPtr))
        dq.push_back(std::string(curPtr));
    return dq;
}

std::deque<std::string> splitString(std::string str, const char* splitter, size_t sz = 0)
{ 
    char line[1024] = {0,}, *curPtr = line;
    strcpy(line, str.c_str());
    std::deque<std::string> dq;

    for (char* ptr = line ; *ptr != '\0' ; ptr++) { 
        if (!strncmp(ptr, splitter, strlen(splitter))) {
            *ptr = 0;
            dq.push_back(std::string(curPtr));
            if (sz && sz <= dq.size()) return dq;
            curPtr = ptr + strlen(splitter);
            ptr = curPtr-1;
        }
    }
    if (strlen(curPtr))
        dq.push_back(std::string(curPtr));
    return dq;
}

std::string strip(std::string str)
{
    char line[1024] = {0,}, *ptr = line, *endPtr;
    strcpy(line, str.c_str());
    endPtr = line + strlen(line) - 1;
    for (; *ptr == ' ' ; ptr++);
    for (; *endPtr == ' ' ; endPtr--) {
        *endPtr = '\0';
    }
    return std::string(ptr);
}

#endif