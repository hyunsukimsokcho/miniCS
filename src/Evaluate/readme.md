# Evaluate

소스코드의 파일명과 락이 걸리는 줄의 위치를 입력으로 받아서 감지되는 data race의 개수와 락 사이에 실행되는 machine instruction의 개수를 return 한다.
**파라미터와 return 방법에 대해서는 추가적인 논의 필요.**

## 입력 파일
`test.cpp`, `lock_range.txt`, `test_join.cpp` 이 세 파일이 같은 위치에 있어야 함.
`test.cpp` 는 테스트 코드.
`lock_range.txt` 는 lock 시작, 끝 위치를 공백으로 구분하여 N줄에 기록한 파일
`test_join.txt`는 `test.cpp`에 `pthread_join`을 추가한 코드.

## 출력 형식
`A B` ( 두 정수가 공백으로 구분되어 표준 출력으로 출력됨 )
`A`는 data race 개수, `B`는 critical region에서 실행된 machine instruction 갯수

## 파일 설명
- evaluate.cpp : int evaluate(*filename)을 사용하여 evaluate의 수행이 가능하다.
- script.py : gdb를 이용하여 machine instruction의 개수를 count한다.

## 실행 방법
1. evaluate.cpp와 test할 파일을 같은 위치에 두고
2. ```gcc evaluate.cpp``` ```./a.out```을 실행하거나
(이 경우, evaluate.cpp의 ```main```함수에 따라 테스트할 파일의 이름은 test.cpp 이어야 한다.)
3. 혹은 다른 파일에서 evaluate.cpp 의 int evaluate 함수를 원하는 테스트 코드 이름을 입력하여 실행해도 된다.
(단, clang이 깔려 있어야하며 ```sudo apt install clang```을 실행할 시 문제없이 작동함을 확인하였다.)

## 실행 후 생성되는 파일
- output : 특정 코드에 대한 data race detection을 수행한다. ./output으로 실행하면 out 파일을 생성
- out : output의 data race detection 결과를 파일로 저장한다. src/ErrorMessageParser의 input으로 이용됨
- output.txt : script.py의 gdb 수행 결과를 저장하는 파일. 결과적으로는 machine instruction 개수를 저장할 예정.
