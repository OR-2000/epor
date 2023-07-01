#include "numa.hh"

#include <errno.h>      // errno
#include <sched.h>      // sched_setaffinity
#include <sched.h>      // getcpu
#include <stdio.h>      // fflush
#include <stdlib.h>     // exit
#include <string.h>     // strerror
#include <sys/types.h>  // pid_t

#include <cassert>

void Numa::schedGetaffinity(cpu_set_t& cpu_set) {
  if (sched_getaffinity(pid_, sizeof(cpu_set_old_), &cpu_set_old_)) {
    printf("sched_getaffinity: errno = %d, %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
};

void Numa::cpuSet(cpu_set_t& cpu_set, const int cpu) {
  CPU_ZERO(&cpu_set);      // どのcpuも含まれない状態にする
  CPU_SET(cpu, &cpu_set);  // cpuを追加
}

Numa::Numa(pid_t pid, const int cpu) : pid_(pid) {
  assert(cpu < LOGICAL_CORE_SIZE);
  cpu_set_t cpu_set;
  schedGetaffinity(cpu_set_old_);
  cpuSet(cpu_set, cpu);
  schedSetaffinity(cpu_set);
  getCpu();
  assert(cpu == cpu_);
};

Numa::~Numa() { schedSetaffinity(cpu_set_old_); };

void Numa::schedSetaffinity(cpu_set_t& cpu_set) {
  if (sched_setaffinity(pid_, sizeof(cpu_set), &cpu_set)) {
    printf("sched_setaffinity: errno = %d, %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void Numa::getCpu() {
  if (getcpu(&cpu_, &node_)) {
    printf("getcpu: errno = %d, %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
