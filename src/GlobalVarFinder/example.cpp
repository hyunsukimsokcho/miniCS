double gg = 1234;
int arr[100];
bool foo(int c)
{
    int a = c;
    int b = gg;
    int V = a;
    return true;
}

void bar()
{
    int x = 1;
    for (int i = 0; i < 10; ++i)
    {
        int y = x;
    }
}

int a = 1, b = 2;
int main()
{
    bar();
    int a = 1;
    int c = arr[0];
    if (foo(1))
    {
        bar();
    }
}
