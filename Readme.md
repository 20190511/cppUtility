## Linux 전용 Shell Scriptor :: (2024.02.03 ~ 2024.02.05) - Junhyeong(0511)
### (2024.02.06 ~) - IPC(프로세스간 통신) FIFO vs MessageQueue 연구 중... <-- source ~/.bashrc 미해결
### (2024.02.10 ~ 2024.02.11) : xv6 Assemble 프로그램 제작
- Asemble 과제 파일 다운로드
[xv6 프로젝트](https://github.com/20190511/OS_xv6/tree/main)
- 소스코드 합친 후 debug 입력받기 (없을 시 엔터)
 ![image](https://github.com/20190511/cppUtility/assets/70988272/58f75849-3e9a-464b-8035-ed7e27a23540)
- debug 입력받은 후의 xv6 실행 모습
![파일 입력받기](https://github.com/20190511/cppUtility/assets/70988272/f88bdc2c-5dd6-4ece-bb1b-f3a232c0758f)
![image](https://github.com/20190511/cppUtility/assets/70988272/5987e8f6-f1ca-4065-b3ef-518a0f836efe)
- Ctrl-A 와 x 입력 후 xv6 종료시 tmp 파일 정리
![Ctrl-A + X 입력 후 종료 시 tmp파일 삭제](https://github.com/20190511/cppUtility/assets/70988272/88ba8420-447c-4a90-894c-cbb868f7fd6f)

--- 

### in Source
1. ospath.h         # C/C++ SystemCall을 적당히 섞어 만든 python os와 유사한 역할을 하는 패키지 헤더파일 제작
2. stringExpand.h   # C++ string 클래스 추가구현 (split Customizing) 등등
3. scriptReader.cpp # .jh 스크립트를 자동으로 실행
4. run.py           # .pyjh 스크립트를 자동으로 실행


### 사용법
- [script.tar.gz](https://github.com/20190511/cppUtility/files/14164813/script.tar.gz)

```powershell
# wsl-linux 첫 사용 기준 -> script.tar.gz 다운로드 후 home/<Username> 에 붙여넣고 시작할 것
sudo apt update
sudo apt upgrade
sudo apt-get install make
mkdir script
mv script.tar.gz script/script.tar.gz
cd script
tar -xvzf script.tar.gz
make
```

### 주의할 점
```
1.
  CoTTA 설치 중 Anaconda 설치 중 lisnse 를 accept 하려는 부분까지 읽기 위해서 계속 enter를 치다가
  설치가 제대로 되지 않을 수 있다.

  --> Enter 대신 f 키를 연타하면 <no|yes> 가 뜰 텐데 이때 yes를 누르면 된다.

2.
  설치 도중 오류가 난다면 침착하게 다시 make를 하면된다.

3. 즁건에 튕긴다면 아래의 명령어를 실행하라 (.bashrc 가 제대로 적용되지 않는 오류가 한 번씩 보임 (아나콘다 설치 직후))
source ~/.bashrc
```

### anaconda 설치 환경 (주의)
>> <엔터키> - <f키> - <f키> -  yes - <엔터키> - yes  순서로 눌러주면 된다.
>> (중간에 뻑나면.. cd ~ 한 후 rm -rf anaconda3 를 한 후 cd script 후 다시 make를 하자...
![image](https://github.com/20190511/cppUtility/assets/70988272/abd98a27-867c-4cc4-b9f0-d3e5afdac4b9)
![image](https://github.com/20190511/cppUtility/assets/70988272/30e24a6c-bcca-4a5b-bdd3-21bb770747ec)


### .jh파일 문법 (차후 추가)
![image](https://github.com/20190511/cppUtility/assets/70988272/f4469288-f99b-4c5d-a1ba-cc847e43fdad)
