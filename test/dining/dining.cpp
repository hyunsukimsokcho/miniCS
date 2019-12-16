/*
This is test case for dining philosopher problem.
The test have dead lock.

there can be lock case 1 or 2.
lock case 1: lock for every thread(philosopher)=> 2*(# of philsopher) locks are needed
lock case 2: lock for global function => only 2 locks are needed.
*/

#include "dining.h"

void * thread1(void *n) {
    get_chopstick(1);
    eat(1); /* eat */
    put_chopstick(1);
    return NULL;
}
void * thread2(void *n) {
    get_chopstick(2);
    eat(2); /* eat */
    put_chopstick(2);
    return NULL;
}
void * thread3(void *n) {
    get_chopstick(3);
    eat(3); /* eat */
    put_chopstick(3);
    return NULL;
}
void * thread4(void *n) {
    get_chopstick(4);
    eat(4); /* eat */
    put_chopstick(4);
    return NULL;
}

void * thread5(void *n) {
    get_chopstick(5);
    eat(5); /* eat */
    put_chopstick(5);
    return NULL;
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