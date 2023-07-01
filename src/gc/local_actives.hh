#pragma once

#include <cstdint>
#include <vector>

#include "../definition.hh"

class LocalActives {
 public:
  void scan();
  std::vector<uint64_t> actives_;  // 降順
};