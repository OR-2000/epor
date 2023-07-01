#pragma once

#include <sched.h>

class Numa {
 private:
  cpu_set_t cpu_set_old_;
  pid_t pid_;

  void schedSetaffinity(cpu_set_t&);
  void schedGetaffinity(cpu_set_t&);
  void cpuSet(cpu_set_t&, const int);
  void getCpu();

 public:
  unsigned int cpu_;
  unsigned int node_;
  Numa(pid_t, const int);
  ~Numa();
};