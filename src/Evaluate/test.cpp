#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
  
// Let us create a global variable to change it in threads 
int g = 0;
int a = 0;
pthread_mutex_t lock_x;
  
// The function to be executed by all threads 
void *myThreadFun(void *vargp) 
{ 
    // Store the value argument passed to this thread 
    //int *myid = (int *)vargp; 
  
    // Let us create a static variable to observe its changes 
    static int s = 0; 
  
    // Change static and global variables 
    // pthread_mutex_lock(&lock_x);
    ++s;
    ++g; 
    ++a;
    //pthread_mutex_unlock(&lock_x);
  
    // Print the argument, static and global variables 
    //printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++a, ++g); 
	return NULL;
} 
  
int main() 
{ 
    int i; 
    pthread_t tid; 
  
    // Let us create three threads 
    for (i = 0; i < 3; i++) 
    {
        pthread_create(&tid, NULL, myThreadFun, (void *)&tid); 
    }
  
    return 0;
} 

