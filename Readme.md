## Linux 전용 Shell Scriptor :: (2024.02.03 ~ 2024.02.05) - Junhyeong(0511)
--- 

### in Source
1. ospath.h         # C/C++ SystemCall을 적당히 섞어 만든 python os와 유사한 역할을 하는 패키지 헤더파일 제작
2. stringExpand.h   # C++ string 클래스 추가구현 (split Customizing) 등등
3. scriptReader.cpp # .jh 스크립트를 자동으로 실행
4. run.py           # .pyjh 스크립트를 자동으로 실행


### 사용법
- [파일 다운로드 : script.tar.gz](https://github.com/20190511/cppUtility/files/14164322/script.tar.gz)
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
  CoTTA 설치 중 Anaconda 설치 중 lisnse 를 accept 하려는 부분까지 읽기 위해서 계속 enter를 치다가
  설치가 제대로 되지 않을 수 있다.

  --> Enter 대신 f 키를 연타하면 <no|yes> 가 뜰 텐데 이때 yes를 누르면 된다.
```
