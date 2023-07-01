#include "record.hh"

#include <cassert>
#include <iostream>

#include "../measure/measure_type.hh"

#if GC_MODE == 3
Record::Record() : latest_(new Version), gc_ts_(0){};
#elif GC_MODE == 4
Record::Record() : latest_(new Version), tpr_(1){};
#else
Record::Record() : latest_(new Version){};
#endif

Record::~Record() {
  Version *cur_v = latest_;
  while (true) {
    if (!cur_v->prev_) {
      delete cur_v;
      break;
    }
    Version *prev_v = cur_v->prev_;
    delete cur_v;
    cur_v = prev_v;
  };
};