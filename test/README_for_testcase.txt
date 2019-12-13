version info: python3

< test를 위해 >
각 testname_check.py 실행시 exec 파일 생성후, 결과물 읽어옴.

(지금은) 실행시 각 테스트케이스별 성공시 true, 실패시 false를 가지는 array를 프린트.
추후 사용되기 편한 방향으로 수정할 예정임.



<lock positioning 할때, 고려해야할점> - 위희주

1. main 함수는 전체 thread를 실행시키는 kernel이라고 생각하면 된다.
    -> 따라서 main 내에는 lock을 두면 안된다.

2. 다른 함수들은 각 thread의 main함수를 나타내는 thread1, thread2,, 등의 함수로 표현하였으며
최대한 많은 경우의 수를 나타내기 위해, 다른 thread일지라도 같은 기능을 하는 경우에도 각자 다른 함수로 표현하였음.

3. 일부 테스트의 경우 thread1.. 등의 각 thread별 main 함수 외에도, 여러 thread 에서 공통적으로 사용할 수 있는 함수가 있음.
    -> 이 함수 내에도 lock을 둘 수 있음.