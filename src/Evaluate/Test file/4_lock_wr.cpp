#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t lock;
int j;

void *add(void *arg)
{
    pthread_mutex_lock(&lock);
    pthread_mutex_unlock(&lock);
    int i = 0;

    //pthread_mutex_lock(&lock);
	j = j + 1;
	//pthread_mutex_unlock(&lock);
	j = j + 1;
    /*while(i < 5)
    {
        printf("%d", j);
        sleep(1);

        i++;
    }

    printf("...Done\n");

    //pthread_mutex_unlock(&lock);*/
    return NULL;
}

void *sub(void *arg)
{
	
	int i = j * 2;
	
	//pthread_mutex_lock(&lock);
	i = j * 2;
	//pthread_mutex_unlock(&lock);
	printf("%d\n",j);
	return NULL;
}

int
main(void)
{
    int err;
    pthread_t t1, t2;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex initialization failed.\n");
        return 1;
    }

    j = 0;

    pthread_create(&t1, NULL, sub, NULL);
	pthread_create(&t2, NULL, add, NULL);
	
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
