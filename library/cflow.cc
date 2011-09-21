#include <cstdio>
#include <stack>
#define handle_error std::abort

unsigned control_flow_check_counter = 0;
std::stack<unsigned> control_flow_ids;

// Implement control-flow checking as an object
struct control_flow_check
{
  control_flow_check()
    : id(control_flow_check_counter++)
  {
    control_flow_ids.push(id);
  }
  ~control_flow_check()
  {
    if (control_flow_ids.empty() || control_flow_ids.top() != id)
      handle_error();
    control_flow_ids.pop();
  }
private:
  unsigned id;
};

int process(int a, int b)
{
  control_flow_check _1;
  return a + b;
}

// The main application, where redundancy is plugged in using higher-order function
int main()
{
  control_flow_check _1;
  int result = process(1, 2);
  std::printf("The result was %d\n", result);
}
