#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <unordered_map>
#include <time.h>
#include "stringExpand.h"
#include "ospath.h"

using namespace std;
#define FILE_NAME       "vimQuiz/vimTestor"
#define OUT_FILE(BUF, N)    sprintf(BUF, "vimQuiz/quiz_%ld.csv", N);
#define ANS_FILE(BUF, N)    sprintf(BUF, "vimQuiz/answer_%ld.csv", N);
#define ANS_DIR         "vimQuiz"

unordered_map<string, string> wordHash;
priority_queue<pair<int, string>> confuseQ;

bool makeQ() {
    FILE* fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        fprintf (stderr, "%s is None\n", FILE_NAME);
        return false;
    }

    char buf[BUFSIZ];
    while(feof(fp) != -1) {
        if (fgets(buf, BUFSIZ, fp) == NULL) {
            break;
        }
        if (!strstr(buf, "|"))
            break;
        deque<string> q = split(buf, "|");
        wordHash[q[0]] = q[1].substr(0, q[1].length()-1);
        memset(buf, 0, BUFSIZ);
    }
    fclose(fp);
    return true;
}

void makeConfuse() {
    time_t t = time(NULL);
    srand(t);
    
    for (auto item : wordHash) {
        int randSeed = rand() % 100;
        string key = item.first;
        confuseQ.push({randSeed, key});
    }
}

void makeFile() {

    FILE *fp_quiz, *fp_ans;
    time_t t = time(NULL);
    char filename1[100], filename2 [100];
    OUT_FILE(filename1, t);
    ANS_FILE(filename2, t);
    fp_quiz = fopen(filename1, "w"), fp_ans = fopen(filename2, "w");

    if (fp_quiz == NULL) {
        fprintf (stderr, "%s fopen error\n", filename1);
        return;
    }

    if (fp_ans == NULL) {
        fprintf (stderr, "%s fopen error\n", filename2);
        return;
    }
    
    while(!confuseQ.empty()) {
        string key = confuseQ.top().second;
        string value = wordHash[key];
        confuseQ.pop();

        string quiz = key + ",\n";
        string ans = key + "," + value + "\n";
        fputs(quiz.c_str(), fp_quiz);
        fputs(ans.c_str(), fp_ans);
    }
    fclose(fp_ans);
    fclose(fp_quiz);
}

int main(void) {
    makeQ();
    makeConfuse();
    makeFile();
    
    return 0;
}