#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"

int basic(int x)
{
  static unsigned int block = 0;

  int y = 3 * x;

  if (y < 10)
  {
    if (unlikely(block != 0))
      sihft::fault_detected();
    block = 1;
    y += 10;
    goto l2;
  }
  else
  {
l2:
    if (unlikely(block > 1))
      sihft::fault_detected();
    block = 2;
    y -= 10;
  }

  if (unlikely(block < 1 || block > 2))
    sihft::fault_detected();
  block = 0;
  return y;
}
