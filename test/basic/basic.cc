#define likely(x)       __builtin_expect((x),1)

int basic(int input)
{
  int x = input;
  if (likely(x <= 2))
    x += 1;
  return x;
}
