/*

there can be lock case 1 or 2.
lock case 1: lock for every thread(philosopher)=> 2*(# of philsopher) locks are needed
lock case 2: lock for global function => only 2 locks are needed.
*/

#include <stdio.h>  
#include <unistd.h>  
#include <pthread.h>
#include <time.h>


void get_chopstick(int p);
void put_chopstick(int p);
void eat(int p);

void * thread1(void *n);

void * thread2(void *n);
void * thread3(void *n);
void * thread4(void *n);
void * thread5(void *n);

void get_chopstick(int p);

void put_chopstick(int p);

void eat(int p);
