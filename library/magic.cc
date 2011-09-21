#include <cstdlib>
#include <cstdio>
#include <functional>
#include <utility>
#define handle_error std::abort

// Implement the double redundancy as a higher-level function
template <typename Result, typename... Arguments>
std::function<Result(Arguments...)>
double_redundancy(std::function<Result(Arguments...)> f)
{
  return [&](Arguments... args) {
    auto result1 = f(args...);
    asm __volatile__ ("");
    auto result2 = f(args...);
    if (result1 != result2)
      handle_error();
    return result1;
  };
}

// Helper function to promote free function to std::function
template <typename Result, typename... Arguments>
std::function<Result(Arguments...)>
function(Result(&f)(Arguments...))
{
  return std::function<Result(Arguments...)>(f);
}


// The main processing
int process(int a, int b)
{
  return a + b;
}

// The main application, where redundancy is plugged in using higher-order function
int main()
{
  int result = double_redundancy(function(process))(1, 2);
  std::printf("The result was %d\n", result);
}
