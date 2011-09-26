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

  faulty() {}
  faulty(const T & x) : value(x) {}

  faulty & operator=(const T & x) { value = x; }

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

// TODO: Compound assignment operators

template <typename T>
int under_test(int input)
{
  faulty<T> x = (T)(input);
  if (x <= 2)
    x = x + 1;
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
  int expected = under_test<T>(input);

  sihft::set_fault_detected(throw_on_detected);
  int impact_free = 0, corrupted = 0, crashed = 0;
  int start = 0;
  while (true) {
    global_counter = start++;

    if (!setjmp(retbuf))
    {
      int result = under_test<T>(input);
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
