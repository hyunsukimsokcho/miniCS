/*
This is test case for dining philosopher problem.
The test have dead lock.

there can be lock case 1 or 2.
lock case 1: lock for every thread(philosopher)=> 2*(# of philsopher) locks are needed
lock case 2: lock for global function => only 2 locks are needed.
*/

#include "dining.h"

#define N 5     /* number of philosophers */
#define UNUSING 0
#define USING 1
#define LEFT(i) (i+N-1)%N
#define RIGHT(i) (i)%N
#define SEC_NSEC 1000000000LL 

pthread_mutex_t mutex_lock;
int chopsticks[N] = {UNUSING, };

void * thread1(void *n)
{
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    get_chopstick(1);
    eat(1); /* eat */
    put_chopstick(1);
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
}
void * thread2(void *n)
{
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    get_chopstick(2);
    eat(2); /* eat */
    put_chopstick(2);
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
}
void * thread3(void *n)
{
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    get_chopstick(3);
    eat(3); /* eat */
    put_chopstick(3);
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
}
void * thread4(void *n)
{
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    get_chopstick(4);
    eat(4); /* eat */
    put_chopstick(4);
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
}
void * thread5(void *n)
{
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    get_chopstick(5);
    eat(5); /* eat */
    put_chopstick(5);
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
}

void get_chopstick(int p)
{
    pthread_mutex_lock(&mutex_lock); /* lock case 2 */
    if (chopsticks[LEFT(p)] == UNUSING && chopsticks[RIGHT(p)] == UNUSING)
    {
        chopsticks[LEFT(p)] = USING;
        chopsticks[RIGHT(p)] = USING;
        
        printf("philosopher %d: get chopstick\n", p);
    }
    
}

void put_chopstick(int p)
{
    if (chopsticks[LEFT(p)] != USING || chopsticks[RIGHT(p)] != USING)
    {
        printf("ERROR in philosopher %d\n", p);
    }
    else
    {
        chopsticks[LEFT(p)] = UNUSING;
        chopsticks[RIGHT(p)] = UNUSING;
        printf("philosopher %d: put chopstick\n", p);
        pthread_mutex_unlock(&mutex_lock); /* lock case 2 */
    }
    
}

void eat(int p)
{
    
    if (chopsticks[LEFT(p)] != USING || chopsticks[RIGHT(p)] != USING)
    {
        printf("ERROR in philosopher %d\n", p);
    }
    else
    {
        //sleep(1);
        printf("philosopher %d: eating\n", p);
    }
    
}