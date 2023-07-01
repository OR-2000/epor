#include "timestamp.hh"

#include <cassert>

Timestamp::Timestamp() : ts_(1){};

uint64_t Timestamp::fetchAdd() {
  return __atomic_fetch_add(&ts_, 1, __ATOMIC_SEQ_CST);
};