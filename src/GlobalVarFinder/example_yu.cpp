/*
This is test case for dining philosopher problem.
The test have dead lock.

there can be lock case 1 or 2.
lock case 1: lock for every thread(philosopher)=> 2*(# of philsopher) locks are needed
lock case 2: lock for global function => only 2 locks are needed.
*/

#include <stdio.h>  
#include <unistd.h>  
#include <pthread.h>
#include <time.h>
#define N 5     /* number of philosophers */
#define UNUSING 0
#define USING 1
#define LEFT(i) (i+N-1)%N
#define RIGHT(i) (i)%N
#define SEC_NSEC 1000000000LL 

void get_chopstick(int p);
void put_chopstick(int p);
void eat(int p);
int chopsticks[10];

void * thread1(void *n) {
    get_chopstick(1);
    eat(1); /* eat */
    put_chopstick(1);
    return nullptr;
}
void * thread2(void *n) {
    get_chopstick(2);
    eat(2); /* eat */
    put_chopstick(2);
    return nullptr;
}
void * thread3(void *n) {
    get_chopstick(3);
    eat(3); /* eat */
    put_chopstick(3);
    return nullptr;
}
void * thread4(void *n) {
    get_chopstick(4);
    eat(4); /* eat */
    put_chopstick(4);
    return nullptr;
}

void * thread5(void *n) {
    get_chopstick(5);
    eat(5); /* eat */
    put_chopstick(5);
    return nullptr;
}

void get_chopstick(int p) {
    if (chopsticks[LEFT(p)] == UNUSING && chopsticks[RIGHT(p)] == UNUSING) {
        chopsticks[LEFT(p)] = USING;
        chopsticks[RIGHT(p)] = USING;
        printf("philosopher %d: get chopstick\n", p);
    }
}

void put_chopstick(int p) {
    if (chopsticks[LEFT(p)] != USING || chopsticks[RIGHT(p)] != USING) {
        printf("ERROR in philosopher %d\n", p);
    } else {
        chopsticks[LEFT(p)] = UNUSING;
        chopsticks[RIGHT(p)] = UNUSING;
        printf("philosopher %d: put chopstick\n", p);
    }
}

void eat(int p) {
    if (chopsticks[LEFT(p)] != USING || chopsticks[RIGHT(p)] != USING) {
        printf("ERROR in philosopher %d\n", p);
    } else {
        printf("philosopher %d: eating\n", p);
    }
}
