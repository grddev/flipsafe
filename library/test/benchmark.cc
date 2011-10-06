#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "benchmark_results.h"
#include "benchmark_timer.h"

#define basic auto_dup
#define method dup
#include "basic/auto.hh"
#undef basic
#undef method

#define basic auto_tri
#define method tri
#include "basic/auto.hh"
#undef basic
#undef method

#define basic auto_trump
#define method trump
#include "basic/auto.hh"
#undef basic
#undef method

#define basic hand_dup
#include "basic/dup-man.hh"
#undef basic

#define basic hand_tri
#include "basic/tri-man.hh"
#undef basic

#define basic hand_trump
#include "basic/trump-man.hh"
#undef basic

const unsigned long long iterations = 300000000;

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

  printf("Results %i,%i,%i,%i,%i,%i\n", hdup, adup, htri, atri, htrump, atrump);
}
