#include "data/dup.hh"
int basic(int input)
{
  sihft::dup<int> x = input;
  // Hardcode prediction, so we get consistent results...
  if (likely(x <= 2))
    x += 1;
  return x;
}
