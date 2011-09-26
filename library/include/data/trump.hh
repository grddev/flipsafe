
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
  T original, backup;

public:
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

#define SIHFT_PP_COMPARE(r, t, op) \
  template <typename T, unsigned A> \
  inline bool operator op(const trump<T,A> & x, const T & y) \
  { \
    if (x.original op y) { \
        if (unlikely(!(x.backup op (A*y)))) \
            fault_detected(); \
        return true; \
    } else { \
        if (unlikely(x.backup op (A*y))) \
            fault_detected(); \
        return false; \
    } \
  }
  BOOST_PP_LIST_FOR_EACH(SIHFT_PP_COMPARE, d, (<, (<=, (==, (!=, (>=, (>, BOOST_PP_NIL)))))))
#undef SIHFT_PP_COMPARE
#define SIHFT_PP_COMPARE(r, t, op) \
  template <typename T, unsigned A> \
  inline bool operator op(const trump<T, A> & x, const trump<T, A> & y) \
  { \
    if (x.original op y.original) { \
        if (unlikely(!(x.backup op y.backup))) \
            fault_detected(); \
        return true; \
    } else { \
        if (unlikely(x.backup op y.backup)) \
            fault_detected(); \
        return false; \
    } \
  } \
  BOOST_PP_LIST_FOR_EACH(SIHFT_PP_COMPARE, d, (<, (<=, (==, (!=, (>=, (>, BOOST_PP_NIL)))))))
#undef SIHFT_PP_COMPARE

template<typename T, unsigned A> inline bool operator<(const T & x, const trump<T,A> & y) { return y > x; }
template<typename T, unsigned A> inline bool operator>(const T & x, const trump<T,A> & y) { return y < x; }
template<typename T, unsigned A> inline bool operator<=(const T & x, const trump<T,A> & y) { return y >= x; }
template<typename T, unsigned A> inline bool operator>=(const T & x, const trump<T,A> & y) { return y <= x; }
template<typename T, unsigned A> inline bool operator==(const T & x, const trump<T,A> & y) { return y == x; }
template<typename T, unsigned A> inline bool operator!=(const T & x, const trump<T,A> & y) { return y != x; }

}
