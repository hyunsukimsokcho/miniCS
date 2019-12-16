/* easy test for detecting race condition */
#include "easytest.h"

void * thread1(void *n) {
    int i;
    for (i=0; i<5; i++) {
        printf("thread1: %d\n", cnt);
        cnt ++;
    }
    return NULL;
}

void * thread2(void *n) {
    int i;
    for(i=0; i<5; i++) {
        printf("thread2: %d\n", cnt);
        cnt ++;
    }
    return NULL;
}