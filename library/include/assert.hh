#include "handler.hh"

namespace sihft {

inline void assert(bool condition)
{
  if (!condition) fault_detected();
}

}

