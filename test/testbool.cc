#include "data/epbool.hh"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestData
#include <boost/test/unit_test.hpp>

template <typename T>
void check() {
  static_assert( sizeof(epbool<8,T>::l) >= sizeof(T), "Size lsb" );
  static_assert( sizeof(epbool<8,T>::m) >= sizeof(T), "Size msb" );
  static_assert( epbool<8,T>::l == (T)0x0101010101010101ull, "Lsb" );
  static_assert( epbool<8,T>::m == (T)0x8080808080808080ull, "Msb" );
}

BOOST_AUTO_TEST_CASE(bitcount_test) {
  check<unsigned char>();
  check<unsigned short>();
  check<unsigned int>();
  check<unsigned long>();
  check<unsigned long long>();
}


