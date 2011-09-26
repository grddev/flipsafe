
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>

namespace sihft {

template <typename T>
class tri
  : boost::euclidean_ring_operators< tri<T> >
  , boost::euclidean_ring_operators< tri<T>, T >
  , boost::bitwise< tri<T> >
  , boost::bitwise< tri<T>, T >
  , boost::shiftable< tri<T> >
  , boost::shiftable< tri<T>, T >
  , boost::unit_steppable< tri<T> >
{
public:
  T original, backup1, backup2;

  inline tri() { }
  inline tri(const T & x)
    : original(x)
  {
    volatile T y = x;
    backup1 = y;
    backup2 = y; // Keep separate assignment, to force reread of y
  }

  inline ~tri() {
    assert_valid();
  }

  inline void assert_valid() const {
    if ( unlikely(original != backup1) || unlikely(original != backup2) )
      fault_detected();
  }

  inline operator const T() const {
    assert_valid();
    return original;
  }
  inline operator T() {
    // Repair the original value from the backup if necessary
    if ( unlikely(original != backup1) || unlikely(original != backup2) )
    {
      if ( unlikely(backup1 == backup2) )
        original = backup1;
      else if (original == backup1)
        backup2 = original;
      else if (original == backup2)
        backup1 = original;
      else /* if (all different) */
        fault_detected(); 
    }

    return original;
  }

  inline tri& operator=(const T & x)
  {
    volatile T y = original = x;
    backup1 = y;
    backup2 = y; // Keep separate assignment, to force reread of y
    return *this;
  }

#define SIHFT_PP_ASSIGN(r, t, op) \
  inline tri& operator op##=(const T & x) \
  { \
    original op##= x; \
    backup1 op##= x; \
    backup2 op##= x; \
    return *this; \
  } \
  inline tri& operator op##=(const tri<T> & x) \
  { \
    original op##= x.original; \
    backup1 op##= x.backup1; \
    backup2 op##= x.backup2; \
    return *this; \
  }
  BOOST_PP_LIST_FOR_EACH(SIHFT_PP_ASSIGN, d, (+, (-, (*, (/, (&, (|, (^, (<<, (>>, BOOST_PP_NIL))))))))))
#undef SIHFT_PP_ASSIGN

  inline tri& operator--()
  {
    return *this -= static_cast<T>(1);
  }

  inline tri& operator++()
  {
    return *this += static_cast<T>(1);
  }

};

#define SIHFT_TRI_COMPOPS (10, (+=, -=, *=, /=, %=, |=, &=, ^=, <<=, >>=))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(SIHFT_TRI_COMPOPS)-1)
#define BOOST_PP_FILENAME_1 "data/op/compound_tri.hh"
#include BOOST_PP_ITERATE()

#define SIHFT_TRI_COMPARES  (6, (<, <=, ==, !=, >=, >))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(SIHFT_TRI_COMPARES)-1)
#define BOOST_PP_FILENAME_1 "data/op/compare_tri.hh"
#include BOOST_PP_ITERATE()

}
