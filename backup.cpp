#include <deque>
#include <iostream>
#include <string>
#include "ospath.h"
#include "stringExpand.h"

/** system Heaeder */
#include <time.h>
#include <sys/time.h>

/** openssh Header */
// File 내용 Hasing 비교
#ifndef OPENSSL_API_COMPAT
#define OPENSSL_API_COMPAT      0x10101000L
#endif
#include <openssl/md5.h>
#include <openssl/sha.h>

/** open file control*/
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
using namespace std;

#ifndef BUFSIZE
#define BUFSIZE     1024
#endif


deque<string> searchDir(string p);
void hyperSetup();

/** originPath <--> backup **/
string oriToBackup(string s);
string backupToOri(string s);
deque<string> oriToBackup(deque<string> dq);
deque<string> backupToOri(deque<string> dq);

string oriToBackupOnly(string s);
string backupToOriOnly(string s);

string attachTimeTag(string name);
string detachTimeTag(string name);

/** checksum **/
string  fileToHashMD5(string fname);
string  fileToHashSHA(string fname);

/** A. Backup **/
bool backup(string fname);


/** X. FileCopy **/
bool fileCopy(string from, string to);
bool mkdirs(string dirs);
bool hashCheck(string originPath);
deque<string> backupList(string originPath);


/**backupDir name*/
string  backupDir;
string  curPath;
int     hash_mode = 0;

int main(int argc, char* argv[])
{
    hyperSetup();


    /*
    deque<string> dq = searchDir(".");
    deque<string> newDq = oriToBackup(dq);

    for (int i = 0 ; i < (int)newDq.size() ; i++)
        cout<<newDq.at(i)<<endl;
    */
    
    backup("/home/junhyeong/ScriptReader/ospath.h") ;
    exit(0);
}

bool fileCopy(string from, string to)
{

    if (!mkdirs(to)) {
        return false;
    }

    int fd1, fd2, length;
    char buf[BUFSIZE];
    if ((fd1 = open(from.c_str(), O_RDONLY)) < 0) {
        fprintf (stderr, "fopen error for %s\n", from.c_str());
        return false;
    }

    if ((fd2 = open(to.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0) {
        fprintf (stderr, "fopen error for %s\n", to.c_str());
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
    path p(".");
    deque<string> dq = searchDir(fname);
    if (!p.isFile(fname)) {
        fprintf(stderr, "%s cannot be accessed\n", fname.c_str());
        return false;
    }
    if (!p.isDir(fname)) 
        dq.push_back(fname);

    deque<string> backupQ;
    string backupMotherPath = "";
    for (int i = 0 ; i < (int)dq.size() ; i++) {
        string originPath = dq.at(i);
        string backupPath = oriToBackupOnly(originPath);
        if (backupMotherPath.compare(p.motherDir(backupPath))) {
            backupMotherPath = p.motherDir(backupMotherPath);
            backupQ.clear();
            backupQ = p.readDir(backupMotherPath, false);
        }
        
        //backup 
        if (p.isDir(originPath) && !p.isDir(backupPath)) {
            if (p.isFile(backupPath)) 
                unlink(backupPath.c_str());
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
    path p(".");
    deque<string> bq = p.readDir(p.motherDir(backupPath), false), retQ;

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
    path p(".");
    string dirList = p.motherDir(dirs), mother = "";
    deque<string> dirQ = split(dirList, "/"); 

    while(!dirQ.empty()) {
        mother += string("/") + dirQ.front();
        dirQ.pop_front();

        if (p.isDir(mother)) {
            continue;
        }
        else if (p.isFile(mother))
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
