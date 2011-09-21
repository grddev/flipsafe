
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>

namespace sihft {

template <typename T>
class tri
  : boost::ordered_euclidean_ring_operators< tri<T> >
  , boost::ordered_euclidean_ring_operators< tri<T>, T >
  , boost::bitwise< tri<T> >
  , boost::bitwise< tri<T>, T >
  , boost::shiftable< tri<T> >
  , boost::shiftable< tri<T>, T >
  , boost::unit_steppable< tri<T> >
{
  T original;
  volatile T backup1, backup2;

public:
  tri() { }
  tri(const tri<T> & x) : original(x.original), backup(x.backup1), backup(x.backup2) { }
  tri(const T & x) : original(x), backup1(x), backup2(x) { }

  ~tri() {
    assert_valid();
  }

  void assert_valid() const {
    if ( unlikely(original != backup1) && unlikely(original != backup2) )
      fault_detected();
  }

  inline operator T() const {
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

#define ASSIGN(r, t, op) \
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
  BOOST_PP_LIST_FOR_EACH(ASSIGN, d, (, (+, (-, (*, (/, (&, (|, (^, (<<, (>>, BOOST_PP_NIL)))))))))))

  inline tri& operator--()
  {
    return *this -= static_cast<T>(1);
  }

  inline tri& operator++()
  {
    return *this += static_cast<T>(1);
  }

};

}
