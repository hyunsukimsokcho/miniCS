int x = 12;
void foo(){
  int y = x++;
  int z = --x;
  int w = -x;
  int Q = -x + x++ - x++;
  y++;
  --z;
  x++;
  x = 13;
  x+= y;
  y+= x;
  z+= x++;
  y = x;
}

int main()
{
    foo();
}
