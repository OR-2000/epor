#pragma once

#include <cstdint>

#include "access_counter.hh"
#include "../definition.hh"

class Lock {
 public:
  Lock(AccessCounter &);
  ~Lock();

  void lock() {
    while (!tryLock()) cpu_relax();
  }

 private:
  AccessCounter &counter_;
  void unlock() { counter_.atomicStoreZero(); }
  bool tryLock();
};