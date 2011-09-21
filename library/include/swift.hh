
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>

namespace sihft {

template <typename T>
class swift
  : boost::ordered_euclidean_ring_operators< swift<T> >
  , boost::ordered_euclidean_ring_operators< swift<T>, T >
  , boost::bitwise< swift<T> >
  , boost::bitwise< swift<T>, T >
  , boost::shiftable< swift<T> >
  , boost::shiftable< swift<T>, T >
  , boost::unit_steppable< swift<T> >
{
  T original;
  volatile T backup;

public:
  swift() { }
  swift(const swift<T> & x) : original(x.original), backup(x.backup) { }
  swift(const T & x) : original(x), backup(x) { }

  ~swift() {
    assert_valid();
  }

  void assert_valid() const {
    if ( unlikely(original != backup) )
      fault_detected();
  }

  inline operator T() const {
    return original;
  }

#define ASSIGN(r, t, op) \
  inline swift& operator op##=(const T & x) \
  { \
    original op##= x; \
    backup op##= x; \
    return *this; \
  } \
  inline swift& operator op##=(const swift<T> & x) \
  { \
    original op##= x.original; \
    backup op##= x.backup; \
    return *this; \
  }
  BOOST_PP_LIST_FOR_EACH(ASSIGN, d, (, (+, (-, (*, (/, (&, (|, (^, (<<, (>>, BOOST_PP_NIL)))))))))))

  inline swift& operator--()
  {
    return *this -= static_cast<T>(1);
  }

  inline swift& operator++()
  {
    return *this += static_cast<T>(1);
  }

};

/*
template <typename T>
class swift_r
{
  T original;
  volatile T backup1, backup2;

public:
  swift_r(const T & x) : original(x), backup1(x), backup2(x)
  {
  }

  operator T() const
  {
    bool c1 = original == backup1;
    bool c2 = original == backup2;
    if ( unlikely(!c1 && c2 ) )
      backup1 = original;
    else if ( unlikely(c1 && !c2) )
      backup2 = original;
    else if ( unlikely(!c1 && !c2) )
    {
      if (backup1 == backup2)
        original = backup1;
      else
        fault_detected();
    }
    return original;
  }

};

template <typename T, unsigned A = 3>
class trump
{
  T original;
  volatile T backup;

public:
  trump(const T & x) : original(x), backup(A*x)
  {
    // Assert that the multiplication does not overflow
    assert(backup/A == original);
    // Theoretically possible to check size of input, but
    // would require different cases depending on number
    // of bits and whether data is signed or not.
  }

  operator T() const
  {
    if ( unlikely(A*original != backup) )
    {
      if (backup % A == 0)
        original = backup / A;
      else
        backup = A * original;
    }
    return original;
  }

};

*/

}
