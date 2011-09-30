#include <setjmp.h>
#include <climits>
#include <iostream>
#include "data.hh"
#include "handler.hh"

static int global_counter;

template <typename T>
struct faulty
{
  mutable T value;

  inline faulty() {}
  template <typename X>
  inline faulty(const X & x) : value(x) {}

  inline faulty & operator=(const T & x) { value = x; }

  inline void inject() const
  {
    if (global_counter >= 0)
    {
      const int bitcount = CHAR_BIT * sizeof(T);
      if (global_counter < bitcount)
      {
        // std::cout << "Inject" << std::endl;
        ((char *)(T*)&value)[global_counter / CHAR_BIT] ^= (1 << (global_counter % CHAR_BIT));
      }
      global_counter -= bitcount;
    }
  }


  operator const int () const
  {
    inject();
    return value;
  }

};

#define COMPOPS (10, (+=, -=, *=, /=, %=, |=, &=, ^=, <<=, >>=))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(COMPOPS)-1)
#define BOOST_PP_FILENAME_1 "compound_faulty.hh"
#include BOOST_PP_ITERATE()

template <typename T>
inline int under_test(int input)
{
  T x = (T)input;
  if (x <= 2)
    x += 1;
  return x;
}

int auto_under_test_dup_int(int input)
{
  return under_test< sihft::dup<int> >(input);
}

int manual_under_test_dup_int(int input)
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

int auto_under_test_tri_int(int input)
{
  return under_test< sihft::tri<int> >(input);
}

int manual_under_test_tri_int(int input)
{
  int x = input, y = input, z = input;
  asm ("" : "+r" (y), "+r" (z));
  if (x <= 2 && likely(y <= 2) && likely(z <= 2) )
  {
    x += 1; y += 1; z += 1;
  }
  /* else ... (see manual_under_test_dup_int) */
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

int auto_under_test_trump_int(int input)
{
  return under_test< sihft::trump<int> >(input);
}

int manual_under_test_trump_int(int input)
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

jmp_buf retbuf;
void throw_on_detected()
{
  longjmp(retbuf, 1);
}

template <typename T>
void test_case()
{
  global_counter = -1;
  int input = 1;
  int expected = under_test< faulty<T> >(input);

  sihft::set_fault_detected(throw_on_detected);
  int impact_free = 0, corrupted = 0, crashed = 0;
  int start = 0;
  while (true) {
    global_counter = start++;

    if (!setjmp(retbuf))
    {
      int result = under_test< faulty<T> >(input);
      if (global_counter >= 0)
        break;

      if (result == expected)
        impact_free++;
      else
      {
        // std::cout << "Experiment " << start << " expected " << expected << " result " << result << std::endl;
        corrupted++;
      }
    }
    else {
      crashed++;
    }
  }
  while (global_counter < 0);

  std::cout <<
    (start - 1) << ';' <<
    crashed << ';' <<
    impact_free << ';' <<
    corrupted << std::endl;

}

#define TEST_CASE(type) do { std::cout << #type ";"; test_case<type>(); } while (0);

int main()
{
  std::cout << "Type;Injected;Detected;Impactless;Corrupted\n";
  TEST_CASE(int);
  TEST_CASE(float);
  TEST_CASE(sihft::dup<int>);
  TEST_CASE(sihft::dup<float>);
  TEST_CASE(sihft::tri<int>);
  TEST_CASE(sihft::tri<float>);
  TEST_CASE(sihft::trump<int>);
}
