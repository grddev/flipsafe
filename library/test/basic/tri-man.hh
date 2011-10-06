int basic(int input)
{
  int x = input, y = input, z = input;
  asm ("" : "+r" (y), "+r" (z));
  // Hardcode first prediction to factor out branch prediction
  if (likely(x <= 2) /*&& likely(y <= 2) && likely(z <= 2)*/ )
  {
    x += 1; y += 1; z += 1;
  }
  /* else ... (see mandup.hh) */
  if ( unlikely(x != y) || unlikely(x != z) )
  {
    if (y == z)
      return y;
    else if (likely(x == y || x== z))
      return x;
    else
      sihft::fault_detected();
  }
  return x;
}

