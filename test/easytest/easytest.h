/* test using  */

#include <stdio.h>  
#include <unistd.h>  
#include <pthread.h>

void * thread1(void *n);
void * thread2(void *n);

int cnt;
pthread_mutex_t mutex_lock;

