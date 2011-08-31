#include <vector>
#include <iostream>
#include <setjmp.h>


// The idea would be to setjmp both in the constructor and
// the destructor, and then try to longjump to the other
// blocks from this location and ensure that the termination
// handler is called. note that we would need to be clever
// when using setjump, so that the first things that happens
// when we setjmp is that we execute a new setjmp, so that
// we can still return to this location. That shouldn't be
// so difficult, as the return value from setjmp should be
// one when a jump is actually taken, and zero otherwise.
// Unfortunately, I guess it won't really work properly,
// as the setjmp/longjmp combo also somehow maintains the
// stack, which means that we cannot produce a corrupted
// stack. It probably makes more sense to not store the
// current block on the stack anyhow, so should perhaps
// not be such a big problem.
//

struct cflow_test;
std::vector<cflow_test *> cflow_tests;

struct cflow_test
{
  cflow_test(int id) {
    cflow_tests.push_back(this);
    while (setjmp(buf)) {}
  }
  jmp_buf buf;
};



jmp_buf retbuf;
int compare_block;

struct cflow_check
{
  cflow_check(int id, int before) : id(id), before(before) {
    std::cout << "cflow_check" << std::endl;
    if (before >= 0 && compare_block != before)
    {
      //std::cerr << "enter " << compare_block << " != " << before << ' ' << counter << ' ' << tests << std::endl;
      longjmp(retbuf, 1);
    }
    compare_block = id;
  }
  ~cflow_check() {
    std::cout << "~cflow_check" << std::endl;
    if (compare_block != id)
    {
      //std::cerr << "exit  " << compare_block << " != " << id << ' ' << counter << ' ' << tests << std::endl;
      longjmp(retbuf, 1);
    }
    compare_block = before;
  }
private:
  int id, before;
};

struct cflow_log
{
  cflow_log(int id) : id(id) {
    std::cout << "Before block " << id << std::endl;
  }
  ~cflow_log() {
    std::cout << "After block " << id << std::endl;
  }
private:
  int id;
};

std::vector<bool> jmpset(7, false);
jmp_buf jmpbuf[7];

#define CASE(n) case n: longjmp(jmpbuf[n], 1);

#define CASES0 CASE(0)
#define CASES1 CASE(1) CASES0
#define CASES2 CASE(2) CASES1
#define CASES3 CASE(3) CASES2
#define CASES4 CASE(4) CASES3
#define CASES5 CASE(5) CASES4
#define CASES6 CASE(6) CASES5
#define CASES7 CASE(7) CASES6

#define CASES(n) CASES ## n

#define CFLOW_INJECT(n, id) \
  if (counter/n == id && tests++ == 0) switch(counter % n) { CASES(n) } \
  while (setjmp(jmpbuf[id])) {}; jmpset[id] = true;
int counter = 0, tests = 0;

struct hacky_store
{
  hacky_store() {
    std::cout << "hacky_store" << std::endl;
  }
  ~hacky_store() {
    std::cout << "~hacky_store" << std::endl;
  }
};

struct hacky_before : hacky_store {};
struct hacky_after : hacky_store {};

// Using the hacky_test/hacky_store trick, we can make sure that we call setjmp
// just before and just after the calls to the constructor an destructor of the
// specified object. Thus, we can try jumping from before/after any checkpoint
// to before/after any checkpoint easily, using the same scheme as implemented
// with preprocessor magic in this file.
//
// In principle, this would be enough to evaluate any control-flow checking that
// has been implemented using the type system magic. In practice, one would
// definitely need some options in dealing with loops in the procedure under
// test. In the simplest of implementations, one would just generate one jmp_buf
// for each iteration, but that might generate too many tests. This would require
// supplying the hacky_test constructor with a unique id, and somewhere specify
// a policy for handling multiple invocations of a constructor with the same id.
// Should we keep only the first, the first two, the first and the last, or what?
// Implementing these are straightforward.
//
// More pressing for the thesis is comparing the result of type-based implementation
// and direct implementation. For direct implementation this type annotation cannot
// be added to the single type to collect all four jump points. One possibility would
// be to use a type-based annotation on the scope level, which would catch two of
// the four points (after-end and before/after-start depending on positioning).
// If we take the reasonable assumption that the checks will be implemented in an
// assert-like manner, we can simply replace the assert macro like so:
//
//  #define assert(x) (hacky_store(), old_assert(x))
//
// By the virtue of \Cpp semantics, the hacky_store object will be constructed right
// before the invocation of the old_assert and it will be destructed once the expression
// is completely evaluated (that is, after the old_assert). This could obviously also
// be implemented in plain C, by making hack_store into a plain function and defining
// assert as this
//
// #define assert(x) (hacky_store(), old_assert(x), hacky_store())
//
// If the old assert macro is defined to be a statement rather than an expression,
// the above approach doesn't really work properly, we can always resort the classical
// solution with using separate statements
//
// #define assert(x) do { hacky_store(); old_assert(x); hacky_store(); } while(0);
//
// This seems like the most reasonable choice for the direct implementation.

template <typename T>
struct hacky_test : private hacky_before, public T, private hacky_after
{
  template <typename... Args>
  hacky_test(Args... args) : hacky_before(), T(args...), hacky_after() {
    std::cout << "sizeof: " << sizeof(*this) << " " << sizeof(T) << std::endl;
  }
};

void p()
{
CFLOW_INJECT(7, 0)
  cflow_log l1(1);
  hacky_test<cflow_check> c1(1, -1);
CFLOW_INJECT(7, 1)
  if (true) {
CFLOW_INJECT(7, 2)
    cflow_log l2(2);
    cflow_check c2(2, 1);
CFLOW_INJECT(7, 3)
  }
  else
  {
CFLOW_INJECT(7, 4)
    cflow_check c3(3, 1);
CFLOW_INJECT(7, 5)
  }
CFLOW_INJECT(7, 6)
}

int main()
{

  tests = 1;
  p();

  for (counter = 0; counter < 7*7; counter++)
  {
    if (setjmp(retbuf)) {
      std::cout << "Success (jump detected)" << std::endl;
      continue;
    }

    // These are not executed, so testing them will fail...

    if (counter/7 == counter %7)
      continue;

    std::cout << "> Jump from " << counter/7 << " to " << counter%7 << ": ";
    tests = 0;
    if (jmpset[counter%7]) p();
    std::cout << ((tests == 0) ? "No jump" : "Failed (jump was not detected)") << std::endl;
  }

}
