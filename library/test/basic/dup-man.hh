int basic(int input)
{
  int x = input;
  int y = input;
  asm ("" : "+r" (y));
  if (x <= 2)
  {
    if ( !likely(y <= 2) )
      sihft::fault_detected();
    x += 1; y += 1;
  }
  /* The following else can be optimized away, as (x > 2 && y <= 2) implies (x != y),
   * which means that the remaining consistency check is enough. Sadly, neither
   * gcc 4.6.1 nor clang 2.9 automatically recognizes this. Thus, this constitutes
   * the main improvement of the manually implemented version. For the dup test,
   * this saves a comparison and a conditional jump even when no error is present.
   * For trump, a (cheap) multiplication is additionally optimized away. For tri,
   * two comparisons and conditional jumps are optimized away. This causes a few bytes
   * of instruction storage overhead, as well as a few cycles of execution overhead.
   * With the usage of likely/unlikely, the compiler should be able to emit code such
   * that branch prediction works reliably when there are no bit-errors, thus inducing
   * only minimal overhead. In the case of error, these overheads will induce another
   * pipeline stall, but for dup this is offset by avoiding the final check. For tri
   * and trump, the error will be repaired and then the final consistency check will
   * run on the repaired value.
   *//*
  else if (unlikely(y <= 2))
    sihft::fault_detected();
   */
  if ( unlikely(x != y) )
    sihft::fault_detected();
  return x;
}

