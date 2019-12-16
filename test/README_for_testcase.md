# test code for multi-threading in c

These are test codes. To implement multi-thread program in C, we used POSIX pthreads. The test codes have no lock, so the test code may include the dead lock.

## test file
there are __ files for each test.
1. easytest.c
2. DiningP.c
3. global_sturucture.c


### requriments
python3
pthread.h


### how to run
```
python3 [testfile].py
```

or
```
gcc -o ./[testfile] [testfile].c -lpthread
exec ./[testfile]
```

### advise for lock positioning

1. Consider main function as kernel. So, you cannot put the locks in the main function.

2. For each thread, we named the main function of each thread as threadn(thread1, thread2..). Also, for maximizing the number of the available cases, there are some cases where the same function are regarded as different in the different thread.

3. There are global functions that can be used in different threads. Also, we can position the locks in that function.