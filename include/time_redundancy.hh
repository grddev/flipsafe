#include <functional>
#include <utility>
#include "handler.hh"

namespace sihft {

// Implement the double redundancy as a higher-level function
template <typename Result, typename... Arguments>
std::function<Result(Arguments...)>
time_redundancy(std::function<Result(Arguments...)> f)
{
  return [&](Arguments... args) {
    volatile Result result1 = f(args...);
    volatile Result result2 = f(args...);
    if (result1 != result2)
      fault_detected();
    return result2;
  };
}

}

