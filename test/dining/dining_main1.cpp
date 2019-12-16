/*
This is test case for dining philosopher problem.
The test have dead lock.

there can be lock case 1 or 2.
lock case 1: lock for every thread(philosopher)=> 2*(# of philsopher) locks are needed
lock case 2: lock for global function => only 2 locks are needed.
*/

#include "dining.h"


int main()
{
    pthread_mutex_init(&mutex_lock, NULL);
    int threadId;
    
    pthread_t pthread[5];

    pthread_create(&pthread[0], NULL, thread1, NULL);
    pthread_create(&pthread[1], NULL, thread2, NULL);
    pthread_create(&pthread[2], NULL, thread3, NULL);
    pthread_create(&pthread[3], NULL, thread4, NULL);
    pthread_create(&pthread[4], NULL, thread5, NULL);
    
    for (int i =0; i<5; i++)
    {
        pthread_join(pthread[i], NULL);
    }
    
    
    printf("Done with chopsticks array: %d %d %d %d %d\n",chopsticks[0], chopsticks[1],chopsticks[2],chopsticks[3],chopsticks[4]);
    return 0;

}
