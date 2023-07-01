#pragma once

#include "../db/database.hh"
#include "../definition.hh"
#include "../measure/measure.hh"
#include "../operations.hh"

class Tx {
 public:
  Tx(const int, Measure &);

  void begin();
  uint64_t read(const uint64_t);
  bool write(const uint64_t, const uint64_t);
  void commit();
  void abort();

 private:
  const int id_;
  Database db_;
  Measure &measure_;
  Operations read_set_;
  Operations write_set_;
  uint64_t begin_ts_;
  uint64_t commit_ts_;

  void reset() {
    read_set_.clear();
    write_set_.clear();
  };
};
