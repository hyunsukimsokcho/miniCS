# ErrorMessageParser

**Data Race** 가 발생하는 경우 두 스레드에서 각각 코드의 어느 줄에서 에러가 발생하는지 확인하여 코드 라인의 순서쌍을 출력한다.

`clang`이 스레드 에러를 발견하면 `out.txt` 파일을 출력하는데, 이 출력파일에는 다음과 같은 부분이 있다.

```
WARNING: ThreadSanitizer: data race (pid=123)
  Write of size 4 at 0x0000014ca9b8 by thread T2:
    #0 myThreadFun(void*) /home/ehdkacjswo/no lock/1.cpp:21 (output+0x4b8721)

  Previous write of size 4 at 0x0000014ca9b8 by thread T1:
    #0 myThreadFun(void*) /home/ehdkacjswo/no lock/1.cpp:21 (output+0x4b8721)
```

**Data Race**가 발생하였고, 코드 21번 줄에서 스레드 `T1`과 `T2`가 이 에러를 발생시켰다.

`err_msg_parser.cpp`는 모든 **Data Race**에러에 대해 이 에러가 발생한 코드 라인을 확인하여 순서쌍을 출력한다.

예를 들어, 위의 경우 두 스레드에서 모두 21번 줄에 에러가 발생했으므로 `(21,21)`을 출력한다.
