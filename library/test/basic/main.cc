#include "handler.hh"
#ifdef auto
#include "auto.hh"
#else 
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define TEST(x) #x
#define FULLNAME(x) TEST(x-man.hh)
#include FULLNAME(method)
#endif

