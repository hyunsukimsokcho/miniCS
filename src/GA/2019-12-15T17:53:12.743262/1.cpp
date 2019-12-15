#include <pthread.h>
pthread_mutex_t my_lock;

double gg = 1234;
int arr[100];
bool foo(int c)
{
pthread_mutex_lock(&my_lock);
    int a = c;
    int b = gg;
    int V = a;
pthread_mutex_unlock(&my_lock);
pthread_mutex_lock(&my_lock);
    V = gg;
pthread_mutex_unlock(&my_lock);
    return true;
}

void bar()
{
    int x = 1;
    for (int i = 0; i < 10; ++i)
    {
pthread_mutex_lock(&my_lock);
        gg = x;
pthread_mutex_unlock(&my_lock);
pthread_mutex_lock(&my_lock);
        arr[i] = x*x;
pthread_mutex_unlock(&my_lock);
pthread_mutex_lock(&my_lock);
        arr[i]++;
pthread_mutex_unlock(&my_lock);
pthread_mutex_lock(&my_lock);
        -arr[i];
pthread_mutex_unlock(&my_lock);
    }
}

int a = 1, b = 2;
int main()
{
    pthread_mutex_init(&my_lock, NULL);
    bar();
    int a = 1;
    int c = arr[0];
    if (foo(1))
    {
        bar();
    }
}
