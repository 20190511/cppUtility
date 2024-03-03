#include <iostream>
#include <string>
#include <deque>
#include <cstring>
#include <vector>

//System Header
#include <unistd.h>

using namespace std;

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

#ifndef PRINT_SIZE
#define PRINT_SIZE 10
#endif

#ifndef DEBUG
#define DEBUG false
#endif


bool lcsDriver(string a, string b);
bool printLcsFile(string a, string b);


/** Implementation **/

bool lcsDriver(string a, string b) {
    if (access(a.c_str(), F_OK) < 0) {
        fprintf (stderr, "%s is not existed\n",a.c_str());
        return false;
    }

    if (access(b.c_str(), F_OK) < 0) {
        fprintf (stderr, "%s is not existed\n",b.c_str());
        return false;
    }
    
    return printLcsFile(a, b);
}

bool printLcsFile(string a, string b) {
    char buf[BUFSIZE] = {0,};
    int** lcsArray;
    bool* sameCheckerA;
    bool* sameCheckerB;
    int szA, szB;

    FILE *fp1, *fp2; 
    if ((fp1 = fopen(a.c_str(), "r")) == NULL)  {
        fprintf (stderr, "fopen error for %s\n", a.c_str());
        return false;
    }

    if ((fp2 = fopen(b.c_str(), "r")) == NULL)  {
        fprintf (stderr, "fopen error for %s\n", b.c_str());
        return false;
    } 

    deque<string> lineA, lineB;
    while(!feof(fp1)) {
        if (fgets(buf, BUFSIZE, fp1) == NULL) 
            break;    
        lineA.push_back(buf);
        memset(buf, 0, BUFSIZE);
    }

    while(!feof(fp2)) {
        if (fgets(buf, BUFSIZE, fp2) == NULL)
            break;
        
        lineB.push_back(buf);
        memset(buf, 0, BUFSIZE);
    }
    fclose(fp1);
    fclose(fp2); 

    szA = (int)lineA.size(), szB = (int)lineB.size();
    
    int maxCnt = 0;

    sameCheckerA = (bool*)calloc(szA, sizeof(bool));
    sameCheckerB = (bool*)calloc(szB, sizeof(bool));
    lcsArray = (int**)calloc(szA+1, sizeof(int*));

    //lcsArray 구성
    for (int i = 0 ; i <= szA ; i++) {
        lcsArray[i] = (int*)calloc(szB+1, sizeof(int));
        if (!i) continue;
        for (int j = 1 ; j <= szB ; j++) {
            if (!lineA.at(i-1).compare(lineB.at(j-1))) 
                lcsArray[i][j] = lcsArray[i-1][j-1] + 1;
            else
                lcsArray[i][j] = max(lcsArray[i-1][j], lcsArray[i][j-1]);
        }
    }

    int i=szA ,j=szB;
    deque<string> lcsQ;
    //왼쪽 우선이동 : 앞 파일이 우선이기 때문
    while (i >= 1 && j >= 1) {
        if (lcsArray[i][j] == lcsArray[i-1][j]) i--;
        else if (lcsArray[i][j] == lcsArray[i][j-1]) j--;
        else {
            if (lcsArray[i][j] > lcsArray[i-1][j-1])
                sameCheckerA[i-1] = sameCheckerB[j-1] = true;
            i--,j--;
        }
    }

    deque<int> notSamePos;
    for (int j = 0 ; j < szB ; j++)
        if (!sameCheckerB[j])
            notSamePos.push_back(j);

    int idxA=0, idxB=0;
    while(idxA < szA || idxB < szB) {
        for (int cnt = 0 ; idxA < szA && !sameCheckerA[idxA] ; cnt++, idxA++) {
            if (!cnt) {
                cout<<"======================="<<endl;
                cout<<"<"<<a<<" file Differ Part>"<<endl;
            }
                
            cout<<"["<<idxA+1<<".] "<<lineA.at(idxA);
        } 

        for (int cnt = 0 ; idxB < szB && !sameCheckerB[idxB] ; cnt++, idxB++) {
            if (!cnt) {
                cout<<"======================="<<endl;
                cout<<"<"<<b<<" Differ Part>"<<endl;
            }
                
            cout<<"["<<idxB+1<<".] "<<lineB.at(idxB);
        } 

        for (int cnt = 0 ; 
                (idxA < szA && sameCheckerA[idxA]) && (idxB < szB && sameCheckerB[idxB]) ;
                idxA++, idxB++, cnt++) {
            if (!cnt) {
                cout<<"======================="<<endl;
                //cout<<"{동일한 부분들 ...}"<<endl;
            }

            if (cnt < PRINT_SIZE) {
                if (idxA < szA && sameCheckerA[idxA])
                    cout<<lineA.at(idxA);
                else if (idxB < szB && sameCheckerB[idxB])
                    cout<<lineB.at(idxB);
            }
            if (cnt == PRINT_SIZE-1) 
                cout<<"..."<<endl;
        }
    }    

    cout<<endl<<" >>> Analysis finshed <<< "<<endl;
    return true;
}

#if DEBUG
int main(void) {
    printLcsFile("lcs.cpp", "/home/junhyeong/backup/ScriptReader/lcs.cpp_1240203130428");    
    exit(0);
}
#endif