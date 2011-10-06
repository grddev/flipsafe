#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "benchmark_results.h"
#include "benchmark_timer.h"

#define basic auto_dup
#include "basic/dup-auto.cc"
#undef basic

#define basic hand_dup
#include "basic/dup-hand.cc"
#undef basic

#define basic auto_tri
#include "basic/tri-auto.cc"
#undef basic

#define basic hand_tri
#include "basic/tri-hand.cc"
#undef basic

#define basic auto_trump
#include "basic/trump-auto.cc"
#undef basic

#define basic hand_trump
#include "basic/trump-hand.cc"
#undef basic

#define basic auto_cflow
#include "basic/cflow-auto.cc"
#undef basic

#define basic hand_cflow
#include "basic/cflow-hand.cc"
#undef basic

const unsigned long long iterations = 200000000;

template <typename F>
static int run_test(int start, F f, const char * label) {
  start_timer();
  int r = start;
  for (unsigned long long i = 0; i < iterations; ++i)
    r = f(start);
  record_result( timer(), label );
  return r;
}


int main()
{
  srand(time(NULL));
  int start = rand() % 10;

  int hdup = run_test(start, hand_dup, "hand");
  int adup = run_test(start, auto_dup, "auto");
  summarize("Duplication", 1, iterations, true, false);

  int htri = run_test(start, hand_tri, "hand");
  int atri = run_test(start, auto_tri, "auto");
  summarize("Triplication", 1, iterations, true, false);

  int htrump = run_test(start, hand_trump, "hand");
  int atrump = run_test(start, auto_trump, "auto");
  summarize("TRUMP", 1, iterations, true, false);

  printf("Data %i,%i,%i,%i,%i,%i\n", hdup, adup, htri, atri, htrump, atrump);

  int hcflow = run_test(start, hand_cflow, "hand");
  int acflow = run_test(start, auto_cflow, "auto");
  summarize("Control flow", 1, iterations, true, false);

  printf("Cflow %i,%i\n", hcflow, acflow);
}
