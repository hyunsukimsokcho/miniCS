#include <pthread.h>
pthread_mutex_t my_lock;

double gg = 1234;
int arr[100];
bool foo(int c)
{
    int a = c;
    int b = gg;
    int V = a;
    V = gg;
    return true;
}

void bar()
{
    int x = 1;
    for (int i = 0; i < 10; ++i)
    {
        gg = x;
        arr[i] = x*x;
        arr[i]++;
        -arr[i];
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
