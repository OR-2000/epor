#include "operation.hh"

Operation::Operation(Record *record, const uint64_t index, const uint64_t value,
                     Version *version)
    : record_(record), index_(index), value_(value), version_(version){};
Operation::Operation(const uint64_t index, const uint64_t value)
    : record_(nullptr),
      index_(index),
      value_(value),
      version_(nullptr){};
Operation::Operation(const uint64_t index)
    : record_(nullptr),
      index_(index),
      value_(0),
      version_(nullptr){};