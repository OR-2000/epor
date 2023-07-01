#include "local_actives.hh"

#include <algorithm>
#include <iostream>

#include "../global/actives.hh"
#include "../gflags.hh"

void LocalActives::scan() {
  actives_.clear();
  uint64_t active;
  for (int i = 0; i < FLAGS_thread_size; i++) {
    if (0 < (active = global::actives[i].atomicLoad())) {
      actives_.emplace_back(active);
    }
  }
  std::sort(
      actives_.begin(), actives_.end(),
      [](const uint64_t &x, const uint64_t &y) { return y < x; });  // 降順
};

