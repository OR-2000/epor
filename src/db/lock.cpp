#include "lock.hh"

Lock::Lock(AccessCounter &counter) : counter_(counter) { lock(); };
Lock::~Lock() { unlock(); };

bool Lock::tryLock() {
  uint64_t expected;
  while (true) {
    expected = counter_.atomicLoad();
    if (expected == 1) return false;
    if (counter_.compareAndSwap(expected)) return true;
  }
}
