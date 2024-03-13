## Linux System Supported Utility - Bae Junhyeong(0511)
> 현재 지원중인 Linux System Program
> 1. [Linux Script 해석기 (Interpreter) 제작] 자체파일 .jh 파일을 읽는 스크립트모델 _ ScriptReader.cpp, run.py 
>> A. CoTTA (TENT, Continual test-time adaptation) Deep Learning System Auto Installer 제작 및 시연 COTTA/*.jh \
>> B. xv6 가상 OS Emulator  Auto Installer 및 Assembler 제작. > xv6/make_xv6_workstation.jh\
>> - 모든 .jh, .pyjh 스크립트는 ScriptReader.cpp 가 자동으로 find 해줍니다.
> 2. [xv6 Auto Assembler Module] : xv6 가상 OS Custermizing Module 를 자동으로 합쳐주는 모델\
>  (xv6 내부에서 수정된 파일만 "~/Assign/<Directory Name>/소스코드" 안에 있으면 자동으로 Assemble 해주는 모델)
> 3. [Auto-Backup System] : xv6의 각 종 system Call을 활용하여 ~/backup 디렉토리에 Hash (md5, SHA1) 를 활용하여 \
>  백업된 파일과 다른 version인 경우 백업 진행
>- Header 보조파일들..
> 3. [ospath.h] : Python os 모듈과 같이 SystemCall 을 활용한 OS 모듈 구현\
> (Linux OS 내부 파일 상태 및 디렉토리 상태를 알려주는 모듈)
> 3. [stringExpand.h] : split, replace, strip 등 string 에 좀 더 유연하게 사용가능한 String 확장 헤더
> 4. [lcs.hpp] : 자체제작한 File-Compare 지원 헤더 (LCS Alogrithm 이용)
--- 

# How to Use it?
### 1. use Linux Command And github Link (Recommanded)
```powershell
# 설치가 안되어 있을 경우.
sudo apt-get install git-all make
sudo git clone https://github.com/20190511/cppUtility.git
cd cppUtility
make [option]
```
> makefile option :
> > - make : 자동으로 ScriptReader.cpp 가 실행되면 스크립터 해석기가 실행 (자동으로 Package 설치)
> > - make xv6 : 자동으로 xv6 Assembler 가 설치
> > - make backup : Hashing (SHA1, MD5) 기반 백업시스템 시작.
> > - make clean : 모든 실행파일을 자동으로 찾아서 삭제
> >
---
### 2. Use file
- 해당 파일을 다운로드 받고 >> [script.tar.gz](https://github.com/20190511/cppUtility/files/14164813/script.tar.gz)
```commandline
tar -xvzf script.tar.gz
cd ScriptReader
make [option]
```
---


## Additional Files.. (시연에 필요한 보조파일들)
- Asemble 과제 파일 다운로드 (xv6 추가 설계 파일들) : --> [xv6 프로젝트](https://github.com/20190511/OS_xv6/tree/main)



## 각 Utility 별 시연 모습
### 1. make : ScriptReader.cpp, run.py
> 1. 시작
> ![image](https://github.com/20190511/cppUtility/assets/70988272/578bddf2-a6bc-44db-94f7-67f1a76ad19c)
> 2. 0번을 눌렀다면? -> xv6 자동설치
> 2. 만약 1,2 번을 눌렀다면? -> CoTTA  DL 자동설치
>> -  <엔터키> - <f키> - <f키> -  yes - <엔터키> - yes  순서로 눌러주면 된다.
>> -  (중간에 튕겨지면.. cd ~ 한 후 rm -rf anaconda3 를 한 후 cd script 후 다시 make를 하자...
>> ![image](https://github.com/20190511/cppUtility/assets/70988272/abd98a27-867c-4cc4-b9f0-d3e5afdac4b9)
>> ![image](https://github.com/20190511/cppUtility/assets/70988272/30e24a6c-bcca-4a5b-bdd3-21bb770747ec)
>> ## 주의할 점들.
>> 1.CoTTA 설치 중 Anaconda 설치 중 lisnse 를 accept 하려는 부분까지 읽기 위해서 계속 enter를 치다가
  설치가 제대로 되지 않을 수 있다.
>> --> Enter 대신 f 키를 연타하면 <no|yes> 가 뜰 텐데 이때 yes를 누르면 된다.
>> 2. 설치 도중 오류가 난다면 침착하게 다시 make를 하면된다.
>> 3. 중간에 튕긴다면 아래의 명령어를 실행하라 (.bashrc 가 제대로 적용되지 않는 오류가 한 번씩 보임 (아나콘다 설치 직후))
>>> source ~/.bashrc


### 2. make backup
#### Option 정리..
> ./backup <md5|sha1>
> 1. backup <backup_path\> :: backup file
> 2. recover <backup_path\> [-n <new_name\>] :: recover File
> 3. remove <backup_path\> : recover 할 파일들.
> 4. remove -c :: All Clear
> 4. compare <filename\> [Explict Compare Filenane] :: 파일 비교 Module \
> (자동으로 backup된 파일을 찾아서 비교.)
> ![image](https://github.com/20190511/cppUtility/assets/70988272/13e2318d-8a87-44e0-8924-1556903ceb8b)
> 5. ls, vim, vi, help 지원..


### 3. make xv6 : xv6 Customerize File Assembler
> - 소스코드 합친 후 debug 입력받기 (없을 시 엔터)
> ![image](https://github.com/20190511/cppUtility/assets/70988272/58f75849-3e9a-464b-8035-ed7e27a23540)
> - debug 입력받은 후의 xv6 실행 모습
> ![파일 입력받기](https://github.com/20190511/cppUtility/assets/70988272/f88bdc2c-5dd6-4ece-bb1b-f3a232c0758f)
> ![image](https://github.com/20190511/cppUtility/assets/70988272/5987e8f6-f1ca-4065-b3ef-518a0f836efe)
> - Ctrl-A 와 x 입력 후 xv6 종료시 tmp 파일 정리
> ![Ctrl-A + X 입력 후 종료 시 tmp파일 삭제](https://github.com/20190511/cppUtility/assets/70988272/88ba8420-447c-4a90-894c-cbb868f7fd6f)


### .jh 스크립트 문법
> 1. 
![image](https://github.com/20190511/cppUtility/assets/70988272/f4469288-f99b-4c5d-a1ba-cc847e43fdad)


## Current Status ..
> 1. (2024.02.06 ~) - IPC(프로세스간 통신) FIFO vs MessageQueue 연구 중... <-- source ~/.bashrc 미해결
> 2. (2024.02.10 ~ 2024.02.11) : xv6 Assemble 프로그램 제작
> 3. (2024.03.01 ~ 2024.03.02) : Linux Backup System 제작 
> 4. (2024.03.10 ~ ) : {[외부 프로젝트](https://github.com/20190511/StockProject/blob/master/GetAnaly.py) : 주식 지표별 데이터 분석 진행중...}

## Developer
> Bae Junhyeong (20190511) : SSU, 4th Grade Student, System Programmer (Hoping)
