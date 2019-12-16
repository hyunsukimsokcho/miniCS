/* test using  */

#include "easytest.h"

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