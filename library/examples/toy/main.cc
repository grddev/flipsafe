#include "data.hh"
#include "cflow.hh"
#include "assert.hh"

bool wait_for_input()
{
  return false;
}

bool is_button_pressed()
{
  return false;
}

bool is_switch_pressed()
{
  return false;
}

void launch_missiles()
{

}

#include "baseline.hh"
#include "manual.hh"
#include "declarative.hh"

int main()
{
  example_baseline();
  example_manual();
  example_declarative();
}

