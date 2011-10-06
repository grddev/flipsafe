#define TEST(x) #x
#define FULLNAME(x) TEST(data/x.hh)
#include FULLNAME(method)
int basic(int input)
{
  sihft::method<int> x = input;
  // Hardcode prediction, so we get consistent results...
  if (likely(x <= 2))
    x += 1;
  return x;
}

