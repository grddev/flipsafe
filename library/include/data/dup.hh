
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>

namespace sihft {

template <typename T>
class dup
  : boost::ordered_euclidean_ring_operators< dup<T> >
  , boost::ordered_euclidean_ring_operators< dup<T>, T >
  , boost::bitwise< dup<T> >
  , boost::bitwise< dup<T>, T >
  , boost::shiftable< dup<T> >
  , boost::shiftable< dup<T>, T >
  , boost::unit_steppable< dup<T> >
{
  T original;
  volatile T backup;

public:
  dup() { }
  dup(const dup<T> & x) : original(x.original), backup(x.backup) { }
  dup(const T & x) : original(x), backup(x) { }

  ~dup() {
    assert_valid();
  }

  void assert_valid() const {
    if ( unlikely(original != backup) )
      fault_detected();
  }

  inline operator T() const {
    return original;
  }

#define SIHFT_PP_ASSIGN(r, t, op) \
  inline dup& operator op##=(const T & x) \
  { \
    original op##= x; \
    backup op##= x; \
    return *this; \
  } \
  inline dup& operator op##=(const dup<T> & x) \
  { \
    original op##= x.original; \
    backup op##= x.backup; \
    return *this; \
  }
  BOOST_PP_LIST_FOR_EACH(SIHFT_PP_ASSIGN, d, (, (+, (-, (*, (/, (&, (|, (^, (<<, (>>, BOOST_PP_NIL)))))))))))
#undef SIHFT_PP_ASSIGN

  inline dup& operator--()
  {
    return *this -= static_cast<T>(1);
  }

  inline dup& operator++()
  {
    return *this += static_cast<T>(1);
  }

};

}
