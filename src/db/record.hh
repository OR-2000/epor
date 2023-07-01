#pragma once

#include "../definition.hh"
#include "access_counter.hh"
#include "version.hh"

class CACHE_ALIGNED Record {
 public:
  Record();
  ~Record();

  AccessCounter counter_;
  Version *latest_;
#if GC_MODE == 3
  uint64_t gc_ts_;  // TODO: public
#endif
#if GC_MODE == 4
  uint64_t tpr_;
#endif
};
