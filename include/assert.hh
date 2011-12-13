#pragma once
#include "handler.hh"

namespace sihft {

inline void assert(bool condition)
{
  if (__builtin_expect(!condition, 0)) fault_detected();
}

}

