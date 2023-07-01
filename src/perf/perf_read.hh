#pragma once

#include <cstdint>

class PerfRead {
 public:
  uint64_t refs_;
  uint64_t misses_;
};