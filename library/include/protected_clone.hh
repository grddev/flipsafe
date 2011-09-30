#pragma once

namespace sihft
{

template <typename T>
inline T protected_clone(const T & x)
{
#ifdef __GNUC__
  T y = x;
  // The asm declaration specifies that the value of y
  // is written by the asm-block, and as gcc doesn't look
  // into the (empty) asm block, it no longer is aware that
  // y is a duplicate of x.
  // The constraints ensure that this will remain inbetween
  // the above definition and any use of y, which is exactly
  // what we want.
  // Unfortunately, there seems to be no easy way to make
  // this work with both registers and memory, and therefore
  // the asm constraint forces the result into a register.
  // On platforms where registers are in short supply, this
  // might increase register pressure somewhat.
  asm ("" : "+r" (y));
#else
  // Declaring the variable volatile ensures that the
  // compiler doesn't remember its value (good) but
  // unfortunately also means that the variable will
  // always be put on the stack instead of in a register.
  volatile T y = x;
#endif
  return y;

}

inline float protected_clone(const float & x)
{
  volatile float y = x;
  return y;
}


}

