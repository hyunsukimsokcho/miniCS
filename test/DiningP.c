/*
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

void get_chopstick(int p);
void put_chopstick(int p);
void eat(int p);

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

int main()
{
    pthread_mutex_init(&mutex_lock, NULL);
    int threadId;
    
    pthread_t pthread[5];
    int a=1;

    pthread_create(&pthread[0], NULL, thread1, (void *)&a);
    pthread_create(&pthread[1], NULL, thread2, (void *)&a);
    pthread_create(&pthread[2], NULL, thread3, (void *)&a);
    pthread_create(&pthread[3], NULL, thread4, (void *)&a);
    pthread_create(&pthread[4], NULL, thread5, (void *)&a);
    

    for (int i =0; i<5; i++)
    {
        //st = pthread_timedjoin_np(pthread[i], NULL, &ts);
        pthread_join(pthread[i], NULL);
    }
    
    
    printf("Done with chopsticks array: %d %d %d %d %d\n",chopsticks[0], chopsticks[1],chopsticks[2],chopsticks[3],chopsticks[4]);
    return 0;

    
    
}
// pthread_mutex_init(&mutex_lock, NULL);
// pthread_mutex_lock(&mutex_lock);
// pthread_mutex_unlock(&mutex_lock);
// pthread_mutex_destory(pthread_mutex_t *mutex);