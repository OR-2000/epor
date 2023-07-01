#include "records.hh"

#include <unistd.h>  // gitpid

#include <cassert>

#include "../numa.hh"

#include "../gflags.hh"

Records::Records() : records_(new Record *[FLAGS_record_size]) {
  // const int node_size = 8;
  // const int cpus[node_size] = {0, 4, 28, 32, 56, 60, 84, 88};
  // const int partition_size = FLAGS_record_size / node_size;
  // int done = 0;
  // for (int node = 0; node < node_size; node++) {
  //   Numa numa(gettid(), cpus[node] % LOGICAL_CORE_SIZE);
  //   for (int i = done; i < done + partition_size; i++) {
  //     records_[i] = new Record;
  //   }
  //   done = done + partition_size;
  // }

  for(int i = 0; i < FLAGS_record_size; i++) {
    records_[i] = new Record;
  }
}

Records::~Records() {
  for(int i = 0; i < FLAGS_record_size; i++) {
    delete records_[i];
  }
  delete[] records_;
}
