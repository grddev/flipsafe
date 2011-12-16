#include "data.hh"
#include "cflow.hh"
#include "assert.hh"
#include "cdup.hh"
#include "cppdup.hh"

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

void open_door()
{

}

#include "baseline.hh"
#include "manual.hh"
#include "declarative.hh"
#include "cdupuse.hh"
#include "cppdupuse.hh"

int main()
{
  example_baseline();
  example_manual();
  example_declarative();
  example_cdup();
  example_cppdup();
}

