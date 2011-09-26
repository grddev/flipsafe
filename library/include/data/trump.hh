
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>
#include <iostream>

namespace sihft {

template <typename T, int A = 3>
class trump
  : boost::euclidean_ring_operators< trump<T, A> >
  , boost::euclidean_ring_operators< trump<T, A>, T >
  , boost::bitwise< trump<T, A> >
  , boost::bitwise< trump<T, A>, T >
  , boost::shiftable< trump<T, A> >
  , boost::shiftable< trump<T, A>, T >
  , boost::unit_steppable< trump<T, A> >
{
public:
  T original, backup;

  inline trump() { }
  inline trump(const T & x)
    : original(x)
  {
    volatile T y = A * x;
    backup = y;

    // The multiplication might overflow, which for compile-time constants
    // would be nice to catch during compilation, but that would require a
    // different interface. Theoretically, it would be nicer to detect the
    // overflow, and not do this extra check, but that would be somewhat
    // complicated to do in a type-generic way.
    if ( unlikely((backup / A) != original) )
      fault_detected();
  }

  inline ~trump() {
    assert_valid();
  }

  inline void assert_valid() const {
    if ( unlikely(A*original != backup) )
      fault_detected();
  }

  inline operator const T() const {
    assert_valid();
    return original;
  }
  inline operator T() {
    if ( unlikely(A*original != backup) )
    {
      if (backup % A == 0)
        original = backup / A;
      else
        backup = A * original;
    }

    return original;
  }

  inline trump& operator=(const T & x)
  {
    volatile T y = original = x;
    backup = A * y;
    return *this;
  }

#define SIHFT_PP_ASSIGN(r, t, op) \
  inline trump& operator op##=(const T & x) \
  { \
    original op##= x; \
    backup op##= A * x; \
    return *this; \
  }
  BOOST_PP_LIST_FOR_EACH(SIHFT_PP_ASSIGN, d, (*, (+, (-, (<<, BOOST_PP_NIL)))))
#undef SIHFT_PP_ASSIGN
#define SIHFT_PP_ASSIGN(r, t, op) \
  inline trump& operator op##=(const trump<T> & x) \
  { \
    original op##= x.original; \
    backup op##= x.backup; \
    return *this; \
  }
  BOOST_PP_LIST_FOR_EACH(SIHFT_PP_ASSIGN, d, (, (+, (-, BOOST_PP_NIL))))
#undef SIHFT_PP_ASSIGN

  inline trump& operator *=(const trump<T> & x)
  {
    return *this *= (T)x;
  }

  inline trump& operator <<=(const trump<T> & x)
  {
    return *this <<= (T)x;
  }

  inline trump& operator--()
  {
    return *this -= static_cast<T>(1);
  }

  inline trump& operator++()
  {
    return *this += static_cast<T>(1);
  }

};

#define SIHFT_TRUMP_COMPOPS (10, (+=, -=, *=, /=, %=, |=, &=, ^=, <<=, >>=))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(SIHFT_TRUMP_COMPOPS)-1)
#define BOOST_PP_FILENAME_1 "data/op/compound_trump.hh"
#include BOOST_PP_ITERATE()

#define SIHFT_TRUMP_COMPARES  (6, (<, <=, ==, !=, >=, >))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(SIHFT_TRUMP_COMPARES)-1)
#define BOOST_PP_FILENAME_1 "data/op/compare_trump.hh"
#include BOOST_PP_ITERATE()

}
