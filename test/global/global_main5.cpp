/*
 copy global stucture(which consist of int and string).
 combine global and local variable.
*/

#include <stdio.h>  
#include <pthread.h>
#include <string.h>
#include "global.h"

int main()
{
    extern pthread_mutex_t mutex_lock;
    pthread_mutex_init(&mutex_lock, NULL);
    int threadId;
    
    pthread_t pthread[3];
    

    pthread_create(&pthread[2], NULL, thread1, NULL);
    pthread_create(&pthread[0], NULL, thread2, NULL);
    pthread_create(&pthread[1], NULL, thread3, NULL);
    

    for (int i =0; i<3; i++)
    {
        pthread_join(pthread[i], NULL);
    }
    
    
    return 0;
    
}