#pragma once
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "handler.hh"
#include "protected_clone.hh"

namespace sihft
{

/**
 * Implementation of local control flow checking using signature checks.
 * Uses RAII to attach a block id (signature) to a code sequence. The
 * optional second parameter to the construct specifies the valid id of
 * the predecessor block. The code checks before entering the scope that
 * the global block id matches the desired predecessor (if one is specifed).
 * Once the specified scope has ended, the code checks that the global
 * block id matches the current scope, and then handles the id back to
 * the predecessor. This allows for simple syntax-based implementation of
 * these checks, rather than having to perform control flow analysis and
 * figure out all possible predecessor blocks of a certain block.
 *
 * If a block id mismatch is detected, the fault_detected() function is
 * called, and if it returns, the code will continue as if the ids matched.
 * In most cases, however, it is desirable for fault_detected to not return.
 */
struct cflow_check
{
  cflow_check(int id)
    : id(id), before(compare_block)
  {
    compare_block = protected_clone(id);
  }

  cflow_check(int id, int before)
    : id(id), before(before)
  {
    if (unlikely(compare_block != before))
      fault_detected();
    compare_block = id;
  }

  ~cflow_check() {
    if (unlikely(compare_block != id))
      fault_detected();
    compare_block = before;
  }

  static int compare_block;

private:
  int id, before;
};

}
