#pragma once

#include <cstdint>

#include "../definition.hh"

class CACHE_ALIGNED Active {
 public:
  void atomicStore(const uint64_t ts) {
    __atomic_store_n(&active_, ts, __ATOMIC_SEQ_CST);
  };

  uint64_t atomicLoad() const {
    return __atomic_load_n(&active_, __ATOMIC_SEQ_CST);
  };
  Active();

 private:
  uint64_t active_;
};
