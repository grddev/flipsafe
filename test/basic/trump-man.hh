int basic(int input)
{
  int x = input;
  int y = 3 * input;
  asm ("" : "+r" (y));
  // Hardcode branch prediction to factor it out of the performance tests.
  // Also, we can comment out the second condition, as this is automatically
  // checked by the consistency check in the end.
  if (likely(x <= 2) /*&& likely(y <= 6)*/ )
  {
    x += 1; y += 3;
  }
  /* else ... (see manual_under_test_dup_int) */
  if ( !likely(3 * x == y) )
    if (y % 3 == 0)
      x = y / 3;

  return x;
}

