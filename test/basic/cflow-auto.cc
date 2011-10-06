#include "cflow.hh"

int basic(int x)
{
  sihft::cflow_check c0(0);

  int y = 3 * x;

  if (y < 10)
  {
    sihft::cflow_check c1(1, 0);
    y += 10;
    goto l2;
  }
  else
  {
l2:
    sihft::cflow_check c1(2, 0);
    y -= 10;
  }

  return y;
}
