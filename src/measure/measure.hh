#pragma once

#include "../definition.hh"
#include "../gc/time_stamp_counter.hh"
#include "../perf/perf.hh"
#include "measure_type.hh"

class Measure {
 private:
  const int id_;
  unsigned int node_;
  int curSec_;
  Perf* perf_;
  TimeStampCounter tsc_;
  uint64_t cnts_[EXEC_SEC + 1][MeasureType::Size];

 public:
  Measure(const int, unsigned int);
  ~Measure();
  void increment(MeasureType type) { cnts_[curSec_][type]++; };
  void readPerf();
  void start();
  void check();
};