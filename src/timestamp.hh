#pragma once
#include <cstdint>

#include "definition.hh"
class CACHE_ALIGNED Timestamp {
 private:
  uint64_t ts_;

 public:
  Timestamp();
  uint64_t fetchAdd();
};