#include <functional>
#include <utility>
#include "handler.hh"
#include "protected_clone.hh"

namespace sihft {

template <typename Function, typename... Arguments>
inline auto
time_redundancy(Function f, Arguments... args) -> decltype(f(args...))
{
  auto result1 = protected_clone(f(args...));
  asm volatile ("");
  auto result2 = protected_clone(f(args...));
  if (result1 != result2)
    fault_detected();
  return result2;
}

}

