#ifndef  BACKUP_HPP
#define  BACKUP_HPP
#include <deque>
#include <iostream>
#include <string>
/** Cpp Utility Header (junhyeong) **/
#include "ospath.h"
#include "stringExpand.h"


// File 내용 Hasing 비교 
#ifndef OPENSSL_API_COMPAT
//Hash Deprecated Alarm 끄기를 위한 Version 설정
#define OPENSSL_API_COMPAT      0x10101000L
#endif
/** openssh Header */
#include <openssl/md5.h>
#include <openssl/sha.h>

/** open file control System Header*/
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

/** For Time system Heaeder **/
#include <time.h>
#include <sys/time.h>

using namespace std;

#ifndef DEBUG
#define DEBUG       true
#endif

#ifndef BUFSIZE
#define BUFSIZE     1024
#endif

#ifndef PROMPT  
#define PROMPT      cout<<"junhyeong> "
#endif


deque<string> searchDir(string p);
void hyperSetup();

/** originPath <--> backup **/
string oriToBackup(string s); //originFile -> backup (with Time Tag)
string backupToOri(string s); //backupFile -> backup (with Time Tag)
deque<string> oriToBackup(deque<string> dq); // originFile -> backup Queue (with Time Tag)
deque<string> backupToOri(deque<string> dq); // bkacup -> originFile Queue (with Time Tag)

//No Time Tag Transformation 
string oriToBackupOnly(string s); 
string backupToOriOnly(string s);

//Attatch-Detach Time Tag Function
string attachTimeTag(string name);
string detachTimeTag(string name);

/** checksum **/
string  fileToHashMD5(string fname); //hash_mode == 0
string  fileToHashSHA(string fname); //hash_mode == 1

/** A. Backup **/
bool backup(string fname); //backup Module : dir&Reg File All Supported

/** B. Recover **/
bool recover (string fname, string newName);
bool recoverFile (string fname, string newName, bool printUsageFlag); //Each File Recover Function

/** C. Remove **/
bool b_remove (string fname, bool allClear);

/** X. FileCopy **/
bool fileCopy(string from, string to);
bool mkdirs(string dirs);
bool hashCheck(string originPath);
deque<string> backupList(string originPath);
deque<deque<string>> backupClassify(string originPath);

/** XI. USAGE **/
void printUsage();

/** Total Interface **/
void getoptProcess();


/** All Function Used Static Variable **/
string  backupDir;      // BackupPath : Cur, /home/<UserName>/backup
string  curPath;        // curDirectory Path : getcwd(NULL, 0);
path pModule(".");      // Only Used Path Utility
int     hash_mode = 0;  // Choose Hash_mode (0 : md5 , 1: SHA)
void (*help)() = printUsage;

#if DEBUG
int main(int argc, char* argv[])
{
    hyperSetup();
    b_remove("/home/junhyeong/success", false);
    //backup("/home/junhyeong/success");
    //recover("/home/junhyeong/success", "") ;

    exit(0);
}
#endif

void printUsage()
{
    cout<< "Usage:"<<endl;
    cout<< "  > backup [FILENAME]"<<endl;
    cout<< "  > remove [FILENAME] [OPTION]"<<endl;
    cout<< "\t-c : clear backup directory"<<endl;
    cout<< "  > recover [FILENAME] [OPTION]"<<endl;
    cout<< "\t-n [NEWNAME] : recover file with new name"<<endl;
    cout<< "  > ls"<<endl;
    cout<< "  > vi"<<endl;
    cout<< "  > vim"<<endl;
    cout<< "  > help"<<endl;
    cout<< "  > exit"<<endl;
}


bool b_remove (string fname, bool allClear = false) 
{
    bool dirFlag = pModule.isDir(oriToBackupOnly(fname)) ? true : false;
    deque<deque<string>> dq = allClear ? backupClassify(string(getenv("HOME"))) : backupClassify(fname);

    int regCnt = 0, dirCnt = 0;
    while(!dq.empty()) {
        deque<string> in_dq = dq.back();
        dq.pop_back();

        if (dirFlag || allClear) {
            while(!in_dq.empty()) {
                string delFile = in_dq.back();
                in_dq.pop_back();
                // File Existence 검사
                if (access(delFile.c_str(), F_OK) < 0)
                    continue;
                // 파일 일괄삭제 인터페이스
                cout<<"\""<<delFile<<"\" backup file removed"<<endl;
                if (pModule.isDir(delFile)) {
                    dirCnt++;
                    rmdir(delFile.c_str());
                }
                else {
                    regCnt++;
                    unlink(delFile.c_str());
                }
            }
            continue;
        }

        int idx = -2;
        deque<string> bckQ = backupList(backupToOri(in_dq.front()));
        if (bckQ.empty()) {
            fprintf(stderr, "backup file %s is not existed\n", fname.c_str());
            continue;
        }
        else if ((int)bckQ.size() == 1)  {
            unlink(bckQ.front().c_str());
            continue;
        }
        cout<<"backup file list of \""<<fname<<"\""<<endl;
        cout<<"Choose file to remove"<<endl;
        cout<<"0. exit"<<endl;
        for (int i = 0; i < (int)bckQ.size(); i++) {
            string bckName = bckQ.at(i), tag = bckName.substr(bckName.length() - 13);
            cout << i + 1 << ". " << tag << "\t" << pModule.fileSize(bckName) << "Bytes" << endl;
        }
        while (idx < -1 || idx >= (int)bckQ.size()) {
            PROMPT;
            cin>>idx;
            --idx;
        }
        if (idx < 0)
            continue;
        string delFile = bckQ.at(idx);
        cout<<"\""<<delFile<<"\" backup file removed"<<endl;
        unlink(delFile.c_str());
    }

    if (dirFlag) 
        rmdir(oriToBackup(fname).c_str());
    if (allClear) {
        if (!regCnt && !dirCnt) 
            cout<<"no file(s) in the backup"<<endl;
        else
            fprintf(stdout, "backup directory cleared (%d regular files and %d subdirectories totlally).", 
                regCnt, dirCnt);
    }
    return true;
}

deque<deque<string>> backupClassify(string originPath)
{
    deque<string> originQ = pModule.readDir(oriToBackupOnly(originPath));
    deque<deque<string>> backupQ;
    if (originQ.empty()) {
        backupQ.push_back(deque<string>({oriToBackup(originPath)}));
        return backupQ;
    }

    while(!originQ.empty()) {
        string s = originQ.front();
        originQ.pop_front();
        if (pModule.isDir(s)) {
            backupQ.push_back(deque<string>({s}));
            continue;
        }
        string detachName = detachTimeTag(s);
        int passFlag = false;
        for (int i = 0 ; i < (int)backupQ.size() ; i++) {
            if (!detachTimeTag(backupQ.at(i).front()).compare(detachName)) {
                backupQ.at(i).push_back(s);
                passFlag = true;
                break;
            }
        }

        if (passFlag)
            continue;
        backupQ.push_back(deque<string>({s}));
    }
    return backupQ;
}
bool recover (string fname, string newName = "") 
{
    if (newName == "")
        newName = fname;

    deque<deque<string>> dq = backupClassify(fname);
    bool dirFlag = pModule.isDir(oriToBackupOnly(fname)) ? true : false;

    for (int i = 0 ; i < (int)dq.size() ; i++) {
        string curObj = dq.at(i).front(), curFile = pModule.isDir(curObj) ? backupToOriOnly(curObj) : backupToOri(curObj), 
            newBckName = dirFlag ? pModule.join({newName, curFile.substr(fname.length()+1)}) : newName;
        if (pModule.isDir(curObj)) {
            if (!mkdirs(newBckName)) {
                fprintf(stderr, "[recover] mkdirs error for %s\n", newBckName.c_str());
                continue;
            }
            continue;
        }
        recoverFile(curFile, newBckName, !dirFlag);    
    }
    return true;
}

bool recoverFile (string fname, string newName="", bool printUsageFlag = true)
{
    if (newName == "")    
        newName = fname;
    
    deque<string> bckQ = backupList(fname);
    string chooseBckName = "";
    
    // File 선택
    if (!(int)bckQ.size()) {
        if (printUsageFlag) printUsage();
        return false;
    }
    else if ((int)bckQ.size() == 1) {
        chooseBckName = bckQ.front();
    }
    else {
        int idx = -1;
        cout<<"backup file list of \""<<fname<<"\""<<endl;
        for (int i = 0; i < (int)bckQ.size(); i++) {
            string bckName = bckQ.at(i), tag = bckName.substr(bckName.length() - 13);
            cout << i + 1 << ". " << tag << "\t" << pModule.fileSize(bckName) << "Bytes" << endl;
        }

        cout<<"Choose file to recover"<<endl;
        while (idx < 0 || idx >= (int)bckQ.size()) {
            PROMPT;
            cin>>idx;
            --idx;
        }
        chooseBckName = bckQ.at(idx);
    }

    // 해시 비교 (file 존재시)
    if (pModule.isRegular(newName)) {
        string originHash = hash_mode == 0 ? fileToHashMD5(newName) : fileToHashSHA(newName);
        string backupHash = hash_mode == 0 ? fileToHashMD5(chooseBckName) : fileToHashSHA(chooseBckName);
        if (!originHash.compare(backupHash)) {
            fprintf(stderr, "%s is same file as %s\n", newName.c_str(), chooseBckName.c_str());
            return false;
        }
    }

    // 파일 백업 
    if (!fileCopy(chooseBckName, newName)) {
        fprintf(stderr, "[recover] Recovor error for %s\n", newName.c_str());
        return false; 
    }
    
    return true;
}

bool fileCopy(string from, string to)
{

    if (!mkdirs(to)) {
        return false;
    }

    int fd1, fd2, length;
    char buf[BUFSIZE];
    if ((fd1 = open(from.c_str(), O_RDONLY)) < 0) {
        fprintf (stderr, "fopen error for %s (read)\n", from.c_str());
        return false;
    }

    if ((fd2 = open(to.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0) {
        fprintf (stderr, "fopen error for %s (write)\n", to.c_str());
        return false;
    }
    
    while((length = read(fd1, buf, BUFSIZE)) > 0) {
        if (write(fd2, buf, length) != length) {
            fprintf(stderr, "write error for %s\n", to.c_str());
            close(fd1), close(fd2);
            return false;
        }
    }

    cout<<to<<" backuped"<<endl;
    close(fd1);
    close(fd2);
    return true;
}

bool backup(string fname)
{
    deque<string> dq = searchDir(fname);
    if (!pModule.isFile(fname)) {
        fprintf(stderr, "%s cannot be accessed\n", fname.c_str());
        return false;
    }
    if (!pModule.isDir(fname)) 
        dq.push_back(fname);

    deque<string> backupQ;
    string backupMotherPath = "";
    for (int i = 0 ; i < (int)dq.size() ; i++) {
        string originPath = dq.at(i);
        string backupPath = oriToBackupOnly(originPath);

        if (backupMotherPath.compare(pModule.motherDir(backupPath))) {
            backupMotherPath = pModule.motherDir(backupMotherPath);
            backupQ.clear();
            backupQ = pModule.readDir(backupMotherPath, false);
        }
        
        //backup 
        if (pModule.isDir(originPath)) {
            if (!pModule.isDir(backupPath)) {
                if (pModule.isFile(backupPath)) 
                    unlink(backupPath.c_str());
            }
            mkdirs(backupPath);
            continue;
        }

        //hash 비교
        if (!hashCheck(originPath)) {
            fprintf(stderr, "%s already backuped\n", originPath.c_str());
            continue;
        }

        backupPath = attachTimeTag(backupPath);
        if (fileCopy(originPath, backupPath) < 0) {
            fprintf(stderr, "%s can't be backuped\n", originPath.c_str());
            continue; 
        }
    }
    return true;
}

/**
 * @warning :  mode (0 : md5), (1 : sha1)
*/
bool hashCheck(string originPath)
{
    deque<string> dq = backupList(originPath);
    string originHash = hash_mode == 0 ? fileToHashMD5(originPath) : fileToHashSHA(originPath);

    while(!dq.empty()) {
        string s = dq.front();
        dq.pop_front();
        string backupHash = hash_mode == 0 ? fileToHashMD5(s) : fileToHashSHA(s);
        if (!originHash.compare(backupHash))
            return false;
    }
    return true;
}

deque<string> backupList(string originPath)
{
    string backupPath = oriToBackupOnly(originPath);
    deque<string> bq = pModule.readDir(pModule.motherDir(backupPath), false), retQ;

    while(!bq.empty()) {
        string s = bq.front();
        bq.pop_front();

        if (!backupPath.compare(detachTimeTag(s))) 
            retQ.push_back(s);
    }
    return retQ;
}

bool mkdirs(string dirs)
{
    string dirList = pModule.motherDir(dirs), mother = "";
    deque<string> dirQ = split(dirList, "/"); 

    while(!dirQ.empty()) {
        mother += string("/") + dirQ.front();
        dirQ.pop_front();

        if (pModule.isDir(mother)) {
            continue;
        }
        else if (pModule.isFile(mother))
            unlink(mother.c_str());
        
        if (mkdir(mother.c_str(), 0755) < 0) {
            fprintf(stderr, "%s can not be created Directory\n", mother.c_str());
            return false;
        }
    }
    return true;
}

void hyperSetup() {
    backupDir = string(getenv("HOME")) + "/backup";
    curPath = getcwd(NULL, 0);
}

string  fileToHashSHA(string fname) {
    char buf[BUFSIZE] = {0,}, resultBuf[SHA_DIGEST_LENGTH*2+1];
    unsigned char hashBuf[40] = {0,};
    int fd, length = true;
    SHA_CTX ctx;
    if ((fd = open(fname.c_str(), O_RDONLY)) < 0) {
        fprintf (stderr, "fopen error for %s\n", fname.c_str());
        return "";
    }

    SHA1_Init(&ctx);
    while ((length = read(fd, buf, BUFSIZE)) > 0)  {
        SHA1_Update(&ctx, buf, length);
    }
    SHA1_Final(hashBuf, &ctx);
    close(fd);

    for (int i = 0 ; i < SHA_DIGEST_LENGTH ; i++) {
        sprintf(resultBuf+(i*2), "%02x", hashBuf[i]);
    }
    return string(resultBuf);
    
}

string fileToHashMD5(string fname)
{
    char buf[BUFSIZE] = {0,}, resultBuf[MD5_DIGEST_LENGTH*2+1];
    unsigned char hashBuf[40] = {0,};
    int fd, length = true;
    MD5_CTX ctx;
    if ((fd = open(fname.c_str(), O_RDONLY)) < 0) {
        fprintf (stderr, "fopen error for %s\n", fname.c_str());
        return "";
    }

    MD5_Init(&ctx);
    while ((length = read(fd, buf, BUFSIZE)) > 0)  {
        MD5_Update(&ctx, buf, length);
    }
    MD5_Final(hashBuf, &ctx);
    close(fd);

    for (int i = 0 ; i < MD5_DIGEST_LENGTH ; i++) {
        sprintf(resultBuf+(i*2), "%02x", hashBuf[i]);
    }
    return string(resultBuf);
}

/**
 * @example _240301204432
*/
string attachTimeTag(string name)
{
    char nameTag[14] = {0,};
    time_t curTime = time(NULL);
    struct tm* tm = localtime(&curTime);
    sprintf(nameTag, "_%02d%02d%02d%02d%02d%02d", 
        tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    name += nameTag;
    return name;
}

string detachTimeTag(string name)
{
    name = name.substr(0, name.length() - 14); 
    return name;
}


string oriToBackupOnly(string s)
{
    return replace(s, getenv("HOME"), backupDir);
}
string backupToOriOnly(string s)
{
    return replace(s, backupDir, getenv("HOME"));
}
string oriToBackup(string s)
{
    return attachTimeTag(oriToBackupOnly(s));
}

string backupToOri(string s)
{
    return detachTimeTag(backupToOriOnly(s));
}

deque<string> oriToBackup(deque<string> dq)
{
    deque<string> newQ;
    for (int cnt = 0 ; cnt < (int)dq.size() ; cnt++) {
        newQ.push_back(oriToBackup(dq.at(cnt)));
    }
    return newQ; 
}

deque<string> backupToOri(deque<string> dq)
{
    deque<string> newQ;
    for (int cnt = 0 ; cnt < (int)dq.size() ; cnt++) {
        newQ.push_back(backupToOri(dq.at(cnt)));
    }
    return newQ; 
}

deque<string> searchDir(string p)
{
    path myPath = path(p);
    if (myPath.getCurPath() == "") {
        fprintf (stderr, "path %s is not existed\n", p.c_str());
        return deque<string>();
    }

    return myPath.readDir();
}
#endif