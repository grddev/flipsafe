#include "handler.hh"
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define A 3
int basic(int input)
{
  int x = input;
  int y = A * input;
  if (likely(x <= 2) )
  {
    x += 1; y += A;
    if ( unlikely(A * x != y) )
      if (y % A == 0)
        x = y / A;
  }
  return x;
}

