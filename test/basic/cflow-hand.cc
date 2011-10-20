#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"

int basic(int input)
{
  static unsigned int block = 0;
  int x = input;
  if (likely(x <= 2))
  {
    if (unlikely(block != 0))
      sihft::fault_detected();
    block = 1;
    x += 1;
  }
  if (unlikely(block > 1))
    sihft::fault_detected();
  block = 0;
  return x;
}
