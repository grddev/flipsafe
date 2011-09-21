
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>

namespace sihft {

template <typename T, unsigned A = 3>
class trump
  : boost::ordered_euclidean_ring_operators< trump<T, A> >
  , boost::ordered_euclidean_ring_operators< trump<T, A>, T >
  , boost::bitwise< trump<T, A> >
  , boost::bitwise< trump<T, A>, T >
  , boost::shiftable< trump<T, A> >
  , boost::shiftable< trump<T, A>, T >
  , boost::unit_steppable< trump<T, A> >
{
  T original;
  volatile T backup;

public:
  trump() { }
  trump(const trump<T,A> & x) : original(x.original), backup(x.backup) {}
  trump(const T & x) : original(x), backup(A * x) { }
  {
    // The multiplication might overflow, which for compile-time constants
    // would be nice to catch during compilation, but that would require a
    // different interface. Theoretically, it would be nicer to detect the
    // overflow, and not do this extra check, but that would be somewhat
    // complicated to do in a type-generic way.
    if ( unlikely(x.backup / A != original) )
      fault_detected();
  }

  void assert_valid() const {
    // Keep this in order to maintain the same interface across types
  }

  inline operator T() const {
    if ( unlikely(A*original != backup) )
    {
      if (backup % A == 0)
        original = backup / A;
      else
        backup = A * original;
    }

    return original;
  }

#define ASSIGN(r, t, op) \
  inline trump& operator op##=(const T & x) \
  { \
    original op##= x; \
    backup op##= x; \
    return *this; \
  } \
  inline trump& operator op##=(const trump<T> & x) \
  { \
    original op##= x.original; \
    backup op##= x.backup; \
    return *this; \
  }
  BOOST_PP_LIST_FOR_EACH(ASSIGN, d, (, (+, (-, (*, (/, (&, (|, (^, (<<, (>>, BOOST_PP_NIL)))))))))))

  inline trump& operator--()
  {
    return *this -= static_cast<T>(1);
  }

  inline trump& operator++()
  {
    return *this += static_cast<T>(1);
  }

};

/*
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
