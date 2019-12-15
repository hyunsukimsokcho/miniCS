/* easy test for detecting race condition */
#include "easytest.h"

int cnt;
pthread_mutex_t mutex_lock;

void * thread1(void *n)
{
    int i;
    //pthread_mutex_lock(&mutex_lock);
    for (i=0; i<5; i++)
    {
        printf("thread1: %d\n", cnt);
        cnt ++;
        //sleep(1);
    }
    //pthread_mutex_unlock(&mutex_lock);
}

void * thread2(void *n)
{
    int i;
    //pthread_mutex_lock(&mutex_lock);
    for(i=0; i<5; i++)
    {
        
        printf("thread2: %d\n", cnt);
        cnt ++;
        //sleep(1);
    }
    //pthread_mutex_unlock(&mutex_lock);
}
