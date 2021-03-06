#include "handler.hh"
using sihft::fault_detected;
#include "dupint.hh"
#include "implicit.hh"
#include <iostream>
#include <boost/preprocessor.hpp>

#define COMPOPS (10, (+=, -=, *=, /=, %=, |=, &=, ^=, <<=, >>=))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(COMPOPS)-1)
#define BOOST_PP_FILENAME_1 "compound.hh"
#include BOOST_PP_ITERATE()

int main()
{
  dupint_ a = 1;
  dupint_ b = 2;

  // Arithmetic operators
  a = b;
  (void)(a + b);
  (void)(a - b);
  (void)+a;
  (void)-a;
  (void)(a * b);
  (void)(a / b);
  (void)(a % b);
  /*
  ++a;
  a++;
  --a;
  a--;
   */

  // Relational operators
  (void)(a == b);
  (void)(a != b);
  (void)(a > b);
  (void)(a < b);
  (void)(a >= b);
  (void)(a <= b);
  
  // Logical operators
  (void)!a;
  (void)(a && b);
  (void)(a || b);

  // Bitwise operators
  (void)~a;
  (void)(a & b);
  (void)(a | b);
  (void)(a ^ b);
  (void)(a << b);
  (void)(a >> b);

  // Compound assignment operators
  a += b;
  a -= b;
  a *= b;
  a /= b;
  a %= b;
  a |= b;
  a ^= b;
  a <<= b;
  a >>= b;

  // Pointer operator
  (void)(int*)&a;

  // Comma operator
  (void)(int)((void)a,b);

  // Sizeof operator (this will return "wrong" values)
  (void)sizeof(a);


  std::cout << "a:" << a.value() << " b:" << b.value() << std::endl;
}
