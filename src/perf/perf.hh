#pragma once

#include <linux/perf_event.h>
#include <sys/types.h>  // pid_t

#include <cstdint>

#include "perf_read.hh"

class Perf {
 private:
  struct read_format {
    uint64_t nr; /* The number of events */
    struct {
      uint64_t value; /* The value of the event */
    } values[2];
  };

  int fdLeader_;
  int fdMember_;
  int perfEventOpen(struct perf_event_attr *, pid_t, int, int, unsigned long);

 public:
  Perf(const int, pid_t);
  ~Perf();
  void perfRead(PerfRead &);
  void enable();
};