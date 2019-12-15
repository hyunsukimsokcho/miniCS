/*
 copy global stucture(which consist of int and string).
 combine global and local variable.
*/

#include <stdio.h>  
#include <pthread.h>
#include <string.h>



void * thread1(void *n);

void * thread2(void *n);

void * thread3(void *n);


void copy_people(struct people * from, struct people * to);

void modify_name(struct people * picked);
