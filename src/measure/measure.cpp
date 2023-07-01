#include "measure.hh"

#include <cassert>
#include <string>

#include "../gflags.hh"
#include "../io/file.hh"
#include "../perf/perf_read.hh"

Measure::Measure(const int id, unsigned int node)
    : id_(id), node_(node), curSec_(0){};

Measure::~Measure() {
  // delete perf_;
  File f("log/" + std::to_string(FLAGS_filename) + ".csv");
  std::string line = "";

  for (int sec = 0; sec < EXEC_SEC; sec++) {
    line = std::to_string(FLAGS_thread_size) + "," + std::to_string(id_) + "," +
           std::to_string(node_) + "," + std::to_string(sec) + ",";  // TODO

    for (int type = 0; type < MeasureType::Size; type++) {  // TODO
      line.append(std::to_string(cnts_[sec][(MeasureType)type]));
      line.append(",");
    };

    line.pop_back();  // TODO: to strip last comma
    f.write(line);
  }
}

void Measure::readPerf() {
  PerfRead read;  // TODO
  perf_->perfRead(read);
  // cnts_[curSec_][MeasureType::PerfCacheRefs] = read.refs_;
  // cnts_[curSec_][MeasureType::PerfCacheMisses] = read.misses_;
}

void Measure::start() {
  tsc_.lap();
  // perf_->enable();
}

void Measure::check() {
  uint64_t elapsedSec = tsc_.getElapsedSecFromLap();
  if (1 <= elapsedSec) {
    // readPerf();
    curSec_ = curSec_ + elapsedSec;
    tsc_.lap();
    if (EXEC_SEC <= curSec_) curSec_ = EXEC_SEC;  // TODO
  }
}
