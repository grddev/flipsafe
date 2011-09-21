#include "data/dup.hh"
#include <boost/test/test_tools.hpp>

int test_main(int, char**)
{
  int x1 = 12345, x2 = 23456;
  sihft::swift<int> y1 = x1, y2 = x2;

  BOOST_CHECK(x1 == y1 && y2 == x2);
  BOOST_CHECK(x1++ == y1++);
  BOOST_CHECK(x1 == y1);
  BOOST_CHECK(x1-- == y1--);
  BOOST_CHECK(x1 == y1);
  BOOST_CHECK((x1 + x1) == (x1 + y1));
  BOOST_CHECK((x1 + x1) == (y1 + y1));
  BOOST_CHECK((x1 + x1) == (y1 + x1));
  BOOST_CHECK((x1 += x1) == (y1 += x1));
  BOOST_CHECK((x1 += x1) == (y1 += y1));

  return 0;
}
