#include "handler.hh"
#include <cstdlib>

namespace sihft {

static fault_detected_handler handler = NULL;

void set_fault_detected(fault_detected_handler f) throw()
{
  handler = f;
}

void fault_detected() {
  if (handler)
    handler();
  else
    abort();
}

}

