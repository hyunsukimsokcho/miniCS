/*
 copy global stucture(which consist of int and string).
 combine global and local variable.
*/
#include "global.h"

pthread_mutex_t mutex_lock;
struct people
{
    int number;
    int id;
    char first[20];
    char last[20];
    char department[30];
    int place;
};

struct people parr[] = {
    {1,20150496,"heeju",'w','computerscience', 301},
    {2,20160303,"jack",'pat','engineerdesign', 302},
    {3,20170304,"nihao",'hi','chemical',303}
};

struct people picked;

int a = 1;
int b = 1;
int c = 1;
int d = 1;

int copy_cnt =0;

void * thread1(void *n)
{
    int local_var1 = 1000;
    
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    for (int j=0; j < 10; j++)
    {
        a += 1;
        b += 1;
    
    }
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
    a += 1;
    local_var1 *= 2;

    for(int j=0; j<10; j++)
    {
        copy_people(&parr[0], &picked);
        
    }
    copy_cnt += 10;

    local_var1 += d; /* using global var*/
    
}
void * thread2(void *n)
{
    int local_var2 = 1000;
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    for (int j=0; j < 10; j++)
    {
        b += 1;
        c += 1;
    }
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
    for(int j=0; j<10; j++)
    {
        copy_people(&parr[1], &picked);
        
    }
    copy_cnt += 10;

    local_var2 += 2;
    
}

void * thread3(void *n)
{
    d = 3;
    for(int j=0; j<10; j++)
    {
        copy_people(&parr[2], &picked);
        
    }
    copy_cnt += 10;
    
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    modify_name(&picked);
    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */
    
}


void copy_people(struct people * from, struct people * to)
{   
    //pthread_mutex_lock(&mutex_lock); /* lock case 1 */
    to->number = from->number;
    to->id = from->id;
    strcpy(to->first, from->first);
    strcpy(to->last, from->last);
    strcpy(to->department, from->department);
    to->place = from->place;

    //pthread_mutex_unlock(&mutex_lock); /* lock case 1 */

    
}

void modify_name(struct people * picked)
{
    char* temp=&picked->first[2];
    char* reduced_temp = &temp[1];

}
