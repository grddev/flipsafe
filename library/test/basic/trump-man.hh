int basic(int input)
{
  int x = input;
  int y = 3 * input;
  asm ("" : "+r" (y));
  if (x <= 2 && likely(y <= 6) )
  {
    x += 1; y += 3;
  }
  /* else ... (see manual_under_test_dup_int) */
  if ( !likely(3 * x == y) )
    if (y % 3 == 0)
      x = y / 3;

  return x;
}

