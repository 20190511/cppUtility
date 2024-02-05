import subprocess
import os
import sys

SCRIPT_NAME = ".pyjh"
PATHCONF = "/"
CMD_LIST = (
    "chdir:", 
    "run:",
    "reChdir:"
)


def pathFullPath(p:str) -> str :
    pList = p.split(PATHCONF)
    print(p)
    pathVector = []
    if chr(126):
        pathVector.append(os.getenv("HOME"))
        pList.pop(0)
    elif p[0] != '/':
        pathVector.append(os.getcwd())

    for element in pList:
        if element == ".":
            continue
        elif element == "..":
            if len(pathVector) > 0:
                pathVector.pop()
        else:
            pathVector.append(element)

    path = "" 
    for p in pathVector:
        path = os.path.join(path, p)
    return path 
            

class PythonScriptor:
    def __init__(self) -> None:
        self.curScriptor = ""
        self.scriptList = []
        self.cmdQ = []
        self.pathQ = []

        self.scriptFinder()
        self.scriptSelector()
        self.scriptorReader()
        self.runScriptorVector()
        

    def scriptFinder(self):
        fileList = list(os.walk(os.getcwd()))
        for a,b,c in fileList:
            for element in c:
                if SCRIPT_NAME in element:
                    self.scriptList.append(os.path.join(a,element))

    def scriptSelector(self):
        if len(self.scriptList) == 0:
            return False

        for idx in range(10):
            if idx > 0:
                print("Retry your number")
            print(" ===== select your runscriptor ===== ")
            for i, e in enumerate(self.scriptList):
                print(f"[{i}]  {e}")

            fidx = int(input("junhyeong >> "))
            if fidx < 0 or fidx >= len(self.scriptList):
                continue
            else:
                self.curScriptor = self.scriptList[fidx]
                return True
        return False

    def scriptorReader(self):
        ''' scriptor Reader : 번역기'''
        with open(self.curScriptor, "r", encoding="utf-8") as f: 
            lists = f.readlines()
            curNum = -1
            for line in lists: 
                cmdFlag = False
                for i, s in enumerate(CMD_LIST):
                    if line.split("#")[0].strip().upper() == s.upper():
                        curNum = i
                        cmdFlag = True
                        break
                if cmdFlag:
                    continue
                
                appendLine = line.split("#")[0].strip()
                if appendLine != "":
                    appendLine = appendLine.replace("~", os.getenv("HOME"))
                    self.cmdQ.append([curNum, line.split("#")[0].strip()]) 


    def runScriptorVector(self):
        if len(self.cmdQ) == 0:
            print("None Command Vector")
            return False
        for cmdNum, cmdString in self.cmdQ:
            if cmdNum == 0:
                self.pathQ.append(os.getcwd())
                cmdString = pathFullPath(cmdString)
                os.chdir(cmdString)
                print(f"change directory --> {os.getcwd()}")
            elif cmdNum == 1:
                #cmdSplitter = cmdString.split(" ")
                #print(f"run system ==> {cmdSplitter}") 
                try:
                    subprocess.run(cmdString, shell=True)
                except Exception:
                    print(f"수동으로 명령어를 입력해주십시오 : {cmdString}")
                    exit(0)
            elif cmdNum == 2:                
                if cmdString == "init":
                    os.chdir(self.pathQ[0])
                    self.pathQ.clear()
                else:
                    os.chdir(self.pathQ.pop())
                    print(f"change directory --> {os.getcwd()}")
            else:
                print("Wrong cmdNumber")
        
        


if __name__ == "__main__": 
    #subprocess.run(['conda', 'activate', 'cotta'], shell=True)
    #subprocess.run(['bash', 'run_cifar10.sh'],shell=True)
    myScript = PythonScriptor()
    print(myScript.curScriptor)

'''
if __name__ == "__main__":
    os.chdir("/home/junhyeong/cotta2/cifar")
    subprocess.run(['conda', 'activate', 'cotta'], shell=True)
    subprocess.run(['bash', 'run_cifar10.sh'])
'''

