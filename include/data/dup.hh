#pragma once
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include "protected_clone.hh"
#include <boost/operators.hpp>
#include <boost/preprocessor.hpp>

namespace sihft {

template <typename T>
class dup
  : boost::euclidean_ring_operators< dup<T> >
  , boost::euclidean_ring_operators< dup<T>, T >
  , boost::bitwise< dup<T> >
  , boost::bitwise< dup<T>, T >
  , boost::shiftable< dup<T> >
  , boost::shiftable< dup<T>, T >
  , boost::unit_steppable< dup<T> >
{
public:
  T original, backup;

  inline dup() { }
  inline dup(const T & x)
    : original(x), backup(protected_clone(x))
  {
  }

  inline ~dup() {
    assert_valid();
  }

  inline void assert_valid() const {
    if ( unlikely(original != backup) )
      fault_detected();
  }

  inline operator T() const {
    assert_valid();
    return original;
  }

  inline dup& operator=(const T & x)
  {
    volatile T y = original = x;
    backup = y; 
    return *this;
  }


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
    inline addressof(dup<T> *d) : d(d), changed(false) {}
    inline ~addressof() {
      if (changed)
        d->backup = d->original;
    }

    inline operator dup<T> *() const { return d; }
    inline operator const T *() const { return &d->original; }
    inline operator T *() {
      changed = true;
      return &d->original;
    }
  private:
    dup<T> *d;
    bool changed;
  };


  inline addressof operator&() { return addressof(this); }

};

#define SIHFT_DUP_COMPOPS (10, (+=, -=, *=, /=, %=, |=, &=, ^=, <<=, >>=))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(SIHFT_DUP_COMPOPS)-1)
#define BOOST_PP_FILENAME_1 "data/op/compound_dup.hh"
#include BOOST_PP_ITERATE()



#define SIHFT_DUP_COMPARES  (6, (<, <=, ==, !=, >=, >))
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_ARRAY_SIZE(SIHFT_DUP_COMPARES)-1)
#define BOOST_PP_FILENAME_1 "data/op/compare_dup.hh"
#include BOOST_PP_ITERATE()


}
