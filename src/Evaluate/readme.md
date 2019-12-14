# Evaluate

소스코드의 파일명과 락이 걸리는 줄의 위치를 입력으로 받아서 감지되는 data race의 개수와 락 사이에 실행되는 machine instruction의 개수를 return 한다.
**파라미터와 return 방법에 대해서는 추가적인 논의 필요.**

## 입력 파일
`test.cpp`, `lock_range.txt`, `test_join.cpp` 이 세 파일이 같은 위치에 있어야 함.
`test.cpp` 는 테스트 코드.
`lock_range.txt` 는 lock 시작, 끝 위치를 공백으로 구분하여 N줄에 기록한 파일
`test_join.txt`는 `test.cpp`에 `pthread_join`을 추가한 코드.

## 출력 형식
정수 `A` 와 `race_set.txt` 파일.
`A`는 lock 사이에서 실행된 machine instruction 갯수,
`race_set.txt` 파일은 서로 race 하는 두 라인을 공백으로 구분하여 N줄에 기록한 파일.

## 파일 설명
- evaluate.cpp : 컴파일 후 실행시 evaluate의 수행이 가능하다.
- gdb_script.py : gdb를 이용하여 machine instruction의 개수를 count한다.

## 실행 방법
1. evaluate.cpp와 test할 파일을 같은 위치에 두고
2. ```g++ evaluate.cpp``` ```./a.out```을 실행한다.
(단, clang이 깔려 있어야하며 ```sudo apt install clang```을 실행할 시 문제없이 작동함을 확인하였다.)

## 실행 후 생성되는 파일
- race_detect : 특정 코드(test.cpp)에 대한 data race detection을 수행한다. ./race_detect으로 실행하면 race_detection.txt 파일을 생성
- race_detection.txt : output의 data race detection 결과. num_ins 함수로 파싱하여 race_set.txt 파일을 생성.
- race_set.txt : 서로 race 하는 두 라인을 공백으로 구분하여 N줄에 기록한 파일.
- num_ins.txt : script.py의 gdb 수행 결과를 통해 lock 사이에서 실행된 instruction 개수를 저장
- test_join_exe : join이 추가된 test_join.cpp 의 executable
