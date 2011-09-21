#include "time_redundancy.hh"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestTimeRedundancy
#include <boost/test/unit_test.hpp>

// Helper function to promote free function to std::function
template <typename Result, typename... Arguments>
std::function<Result(Arguments...)>
function(Result(&f)(Arguments...))
{
  return std::function<Result(Arguments...)>(f);
}

// The main processing
int stateless(int a, int b)
{
  return a + b;
}

int stateful(int a, int b)
{
  static int c = 0;
  return c += a + b;
}

static int error_count = 0;
void error_collector()
{
  error_count++;
}

// The main application, where redundancy is plugged in using higher-order function
BOOST_AUTO_TEST_CASE(timered)
{
  BOOST_CHECK_EQUAL(stateless(1,2), sihft::time_redundancy(function(stateless))(1,2));

  sihft::set_fault_detected(error_collector);
  // This will obviously not be true for any stateful function, as the results
  // might be the same, given that the error_collector just falls through.
  BOOST_CHECK_NE(stateful(1,2), sihft::time_redundancy(function(stateful))(1,2));
  // We should see one error in the error collector though.
  BOOST_CHECK_EQUAL(1, error_count);
}
