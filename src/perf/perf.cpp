#include "perf.hh"

#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <stdlib.h>     // exit
#include <string.h>     // memset
#include <sys/ioctl.h>  // ioctl
#include <unistd.h>     // syscall

Perf::Perf(const int id, pid_t tid) {
  struct perf_event_attr pe;
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_RAW;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = 0x2d3;  // mem_load_l3_miss_retired.remote_dram
  pe.read_format = PERF_FORMAT_GROUP;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;
  pe.exclude_idle = 1;
  fdLeader_ = perfEventOpen(&pe, tid, id, -1, 0);
  if (fdLeader_ == -1) exit(EXIT_FAILURE);
  pe.type = PERF_TYPE_HARDWARE;
  pe.config = PERF_COUNT_HW_CACHE_MISSES;
  fdMember_ = perfEventOpen(&pe, tid, id, fdLeader_, 0);
  ioctl(fdLeader_, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
}
Perf::~Perf() {
  ioctl(fdLeader_, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
  close(fdLeader_);
  close(fdMember_);
}

void Perf::perfRead(PerfRead &r) {
  struct read_format format;
  read(fdLeader_, &format, sizeof(struct read_format));
  r.refs_ = format.values[0].value;
  r.misses_ = format.values[1].value;
}

int Perf::perfEventOpen(struct perf_event_attr *hw_event, pid_t pid, int cpu,
                        int group_fd, unsigned long flags) {
  return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void Perf::enable() {
  ioctl(fdLeader_, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
}
