
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


  /**
   * This even implements the addressof operator to work in
   * a reasonable fashion. We create a temporary addressof
   * struct which can be converted either to a pointer of the
   * underlaying type or to a pointer to the dup type. When
   * converting to a writable data pointer, we assume that
   * this changes the value of the data, and thereby hook
   * into the destructor of the addressof struct to update
   * the backup variable to mirror the original content.
   * 
   * Obviously, the value isn't protected when we give out
   * the pointer to the underlaying object, but since c++
   * guarantees to call the destructor of the temporary at
   * the end of the expression, we will also not incorrectly
   * report the value as destroyed.
   *
   * If we use and update the value in the same expression,
   * and the compiler orders the read after the update, then
   * we won't have had time to update the backup and a spurious
   * error will be reported. These instances should be really
   * rare, but in those cases, the best solution would be to
   * update the modifying function to take a dup<T>* instead
   * of a T*. If that isn't possible, then the whole expression
   * need to be changed to operate on T instead of dup<T>.
   *
   * The protection doesn't work at all if the pointer is stored
   * and used outside the same expression. For these cases, the
   * dup<T> object must be revalidated once the pointer operations
   * are finished.
   */
  struct addressof
  {
    addressof(dup<T> *d) : d(d), changed(false) {}
    ~addressof() {
      if (changed)
        d->backup = d->original;
    }

    operator dup<T> *() const { return d; }
    operator const T *() const { return &d->original; }
    operator T *() {
      changed = true;
      return &d->original;
    }
  private:
    dup<T> *d;
    bool changed;
  };


  addressof operator&() { return addressof(this); }

};

}
