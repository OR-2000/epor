#pragma once

#include "db/record.hh"
#include "db/version.hh"

class Operation {
 public:
  Operation(Record *, const uint64_t, const uint64_t, Version *);
  Operation(const uint64_t, const uint64_t);
  Operation(const uint64_t);

  Record *record_;
  uint64_t index_;
  uint64_t value_;
  Version *version_;
};
