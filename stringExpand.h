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

#endif