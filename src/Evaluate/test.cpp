/* test using  */

#include <stdio.h>  
#include <unistd.h>  
#include <pthread.h>

void * thread1(void *n);
void * thread2(void *n);

int cnt;
pthread_mutex_t mutex_lock;



void * thread1(void *n)
{
    int i;
    //pthread_mutex_lock(&mutex_lock);
    for (i=0; i<5; i++)
    {
        printf("thread1: %d\n", cnt);
pthread_mutex_lock(&mutex_lock);
        cnt ++;
pthread_mutex_unlock(&mutex_lock);
pthread_mutex_lock(&mutex_lock);
        //sleep(1);
    }
pthread_mutex_unlock(&mutex_lock);
    //pthread_mutex_unlock(&mutex_lock);
}

void * thread2(void *n)
{
    int i;
    //pthread_mutex_lock(&mutex_lock);
    for(i=0; i<5; i++)
    {
        
        printf("thread2: %d\n", cnt);
pthread_mutex_lock(&mutex_lock);
        cnt ++;
pthread_mutex_unlock(&mutex_lock);
pthread_mutex_lock(&mutex_lock);
        //sleep(1);
    }
pthread_mutex_unlock(&mutex_lock);
    //pthread_mutex_unlock(&mutex_lock);
}


int main()
{
    pthread_mutex_init(&mutex_lock, NULL);
    int threadId;
    pthread_t pthread[2];
    int status;
    int a=1;

    cnt =0;
    pthread_create(&pthread[0], NULL, thread1, (void *)&a);
    pthread_create(&pthread[1], NULL, thread2, (void *)&a);
    
    pthread_join(pthread[0], NULL);
    pthread_join(pthread[1], NULL);
    

    return 1;

}
// pthread_mutex_lock(&mutex_lock);
// pthread_mutex_unlock(&mutex_lock);
// pthread_mutex_destory(pthread_mutex_t *mutex);