#include <gflags/gflags.h>
#include <unistd.h>

#include <iostream>

#include "client/multi_client.hh"
#include "db/records.hh"
#include "definition.hh"
#include "gc/active.hh"
#include "gflags.hh"
#include "global/global.hh"
#include "io/file.hh"
#include "numa.hh"
#include "timestamp.hh"

void printGlobalFlags() {
  std::cout << "FLAGS_filename: " << FLAGS_filename << std::endl;
  std::cout << "FLAGS_thread_size: " << FLAGS_thread_size << std::endl;
  std::cout << "EXEC_SEC: " << EXEC_SEC << std::endl;
  std::cout << "GC_MODE: " << GC_MODE << std::endl;
  std::cout << "FLAGS_record_size: " << FLAGS_record_size << std::endl;
  std::cout << "FLAGS_skew: " << FLAGS_skew << std::endl;
  std::cout << "BACKOFF_US: " << BACKOFF_US << std::endl;
  std::cout << "FLAGS_request_size: " << FLAGS_request_size << std::endl;
  std::cout << "FLAGS_request_size_long: " << FLAGS_request_size_long
            << std::endl;
  std::cout << "FLAGS_read_ratio: " << FLAGS_read_ratio << std::endl;
  std::cout << "FLAGS_read_ratio_long: " << FLAGS_read_ratio_long << std::endl;
  std::cout << "SLEEP_US_LONG: " << SLEEP_US_LONG << std::endl;
  // params below is useless when GC_MODE is 0.
  std::cout << "FLAGS_actives_update_interval_ms: "
            << FLAGS_actives_update_interval_ms << std::endl;
  std::cout << "FLAGS_tpr_limit: " << FLAGS_tpr_limit << std::endl;
};

void logGlobalFlags() {
  File f("log/info/" + std::to_string(FLAGS_filename) + ".csv");
  f.write(std::to_string(FLAGS_filename) + "," + std::to_string(GC_MODE) + "," +
          std::to_string(EXEC_SEC) + "," + std::to_string(FLAGS_thread_size) +
          "," + std::to_string(FLAGS_record_size) + "," +
          std::to_string(FLAGS_skew) + "," + std::to_string(BACKOFF_US) + "," +
          std::to_string(FLAGS_request_size) + "," +
          std::to_string(FLAGS_request_size_long) + "," +
          std::to_string(FLAGS_read_ratio) + "," +
          std::to_string(FLAGS_read_ratio_long) + "," +
          std::to_string(SLEEP_US_LONG) + "," +
          std::to_string(FLAGS_actives_update_interval_ms) + "," +
          std::to_string(FLAGS_tpr_limit)+","+
          std::to_string(OPT) + "," + std::to_string(OPT_INTERVAL_MS) + "," +
          std::to_string(OPT_DELETION_THRESHOLD) + "," +
          std::to_string(OPT_MAX_COUNT));
}

void initializeGlobalObject() {
  global::records = new Records;
  global::ts = new Timestamp;
  global::actives = new Active[FLAGS_thread_size];
}

void destroyGlobalObject() {
  delete global::records;
  delete global::ts;
  delete[] global::actives;
};

void startExperiment() { MultiClient clients; };

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  printGlobalFlags();
  logGlobalFlags();

  std::cout << "Initialize..." << std::endl;
  initializeGlobalObject();

  std::cout << "Start experiment..." << std::endl;
  startExperiment();
  std::cout << "done... clean" << std::endl;

  destroyGlobalObject();
  std::cout << "Finish" << std::endl;

  return 0;
};