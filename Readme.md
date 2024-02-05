## Linux 전용 Shell Scriptor :: (2024.02.03 ~ 2024.02.05) - Junhyeong(0511)
--- 

### in Source
1. [ospath.h](https://github.com/20190511/cppUtility/blob/main/ospath.h)         # C/C++ SystemCall을 적당히 섞어 만든 python os와 유사한 역할을 하는 패키지 헤더파일 제작
2. [stringExpand.h](https://github.com/20190511/cppUtility/blob/main/stringExpand.h)   # C++ string 클래스 추가구현 (split Customizing) 등등
3. [scriptReader.cpp](https://github.com/20190511/cppUtility/blob/main/scriptReader.cpp) # .jh 스크립트를 자동으로 실행
4. [run.py](https://github.com/20190511/cppUtility/blob/main/run.py)           # .pyjh 스크립트를 자동으로 실행


### 사용법
- [script.tar.gz](https://github.com/20190511/cppUtility/files/14164093/script.tar.gz)
```powershell
# wsl-linux 첫 사용 기준 -> script.tar.gz 다운로드 후 home/<Username> 에 붙여넣고 시작할 것
## 윈도우 기준, 네트워크 아래보면 Linux 라는 것이 있어 여기를 통해 파일 이동이 가능.
## 혹은 \\wsl.localhost\Ubuntu-<버전>\home\<Username> 으로 이동..
## Ex)) \\wsl.localhost\Ubuntu-20.04\home\junhyeong

sudo apt update
sudo apt upgrade
sudo apt-get install make
mkdir script
mv script.tar.gz script/script.tar.gz
cd script
tar -xvzf script.tar.gz
make
```


### SCript .jh 문법
1. 추가예정.. (run: install: chdir: reChdir: [filecheck])
