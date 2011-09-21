#include "data.hh"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestData
#include <boost/test/unit_test.hpp>

template <typename Method>
void data_test()
{
  typedef typename Method::template bind<int>::type Protected;
  int x1 = 12345, x2 = 23456;
  Protected y1 = x1, y2 = x2;

  BOOST_CHECK_EQUAL(x1, y1);
  BOOST_CHECK_EQUAL(y2, x2);
  BOOST_CHECK_NE(y1, x2);
  BOOST_CHECK_NE(y2, x1);

  BOOST_CHECK_EQUAL(x1++, y1++);
  BOOST_CHECK_EQUAL(x1, y1);
  BOOST_CHECK_EQUAL(x1--, y1--);
  BOOST_CHECK_EQUAL(x1, y1);
  BOOST_CHECK_EQUAL(x1 + x1, x1 + y1);
  BOOST_CHECK_EQUAL(x1 + x1, y1 + y1);
  BOOST_CHECK_EQUAL(x1 + x1, y1 + x1);

  BOOST_CHECK_EQUAL(x1 += x1, y1 += x1);
  BOOST_CHECK_EQUAL(x1, 2 * 12345);
  BOOST_CHECK_EQUAL(y1, 2 * 12345);

  BOOST_CHECK_EQUAL(x1 += x1, y1 += y1);
  BOOST_CHECK_EQUAL(x1, 4 * 12345);
  BOOST_CHECK_EQUAL(y1, 4 * 12345);
}

namespace traits
{
  struct dup   { template<typename T> struct bind { typedef sihft::dup<T>   type; }; };
  struct tri   { template<typename T> struct bind { typedef sihft::tri<T>   type; }; };
  struct trump { template<typename T> struct bind { typedef sihft::trump<T> type; }; };
}

#define SIHFT_DATA_TEST(what) \
  BOOST_AUTO_TEST_CASE(data_test_ ## what) { \
    data_test<traits::what>(); \
  }

SIHFT_DATA_TEST(dup);
SIHFT_DATA_TEST(tri);
SIHFT_DATA_TEST(trump);


void update(int * result, int value)
{
  *result = value;
}

BOOST_AUTO_TEST_CASE(data_test_addressof)
{
  sihft::dup<int> duped = 10;

  update(&duped, 0);
  BOOST_CHECK_EQUAL(duped, 0);

  update(&duped, 1);
  BOOST_CHECK_EQUAL(duped, 1);
}
