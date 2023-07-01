#pragma once

#include <cstdint>

class AccessCounter {
 private:
  uint64_t count_;

 public:
  uint64_t atomicLoad() const {
    return __atomic_load_n(&count_, __ATOMIC_SEQ_CST);
  };
  bool compareAndSwap(uint64_t expected) {
    return __atomic_compare_exchange_n(&count_, &expected, 1, false,
                                       __ATOMIC_SEQ_CST,
                                       __ATOMIC_SEQ_CST);  // TODO
  };
  void atomicStoreZero() { __atomic_store_n(&count_, 0, __ATOMIC_SEQ_CST); };
  AccessCounter();
};
