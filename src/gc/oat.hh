#pragma once

#include "../gflags.hh"

#include "../db/version.hh"
#include "../gc/active.hh"
#include "../measure/measure.hh"
#include "local_actives.hh"
#include "time_stamp_counter.hh"

class Oat {
 public:
  void begin(const uint64_t ts) { active_.atomicStore(ts); };
  Oat(const int, Measure &);

  void makeLatestNotAborted(Version *&latest) {
    while(latest->isAborted()) {
      Version *prev = latest->prev_;
      delete latest;
      measure_.increment(MeasureType::DeleteAbort);
      latest = prev;
    }
  }

  bool updateLocalActives() {
    if (FLAGS_actives_update_interval_ms <= tsc_.getElapsedMsFromLap()) {
      actives_.scan();
      tsc_.lap();
      return true;
    };
    return false;
  };

  LocalActives actives_;
  Measure &measure_;
  Active &active_;
  TimeStampCounter tsc_;

  void pruning(Version *v) {
    Version *grand;
    while (v->prev_) {
      grand = v->prev_->prev_;  //
      delete v->prev_;
      measure_.increment(MeasureType::DeleteOat);
      v->prev_ = grand;
    }
  };
#if GC_MODE == 1 || GC_MODE == 2 || GC_MODE == 4
  Version *retrieveNextComitted(Version *v) {
    Version *grand;
    measure_.increment(MeasureType::Urv1);  // touch prev_
    while (v->prev_ && v->prev_->isAborted()) {
      grand = v->prev_->prev_;  //
      delete v->prev_;
      measure_.increment(MeasureType::DeleteAbort);
      v->prev_ = grand;
    }
    return v->prev_;
  }
  Version *retrieveNextVisible(const uint64_t active, Version *visible) {
    Version *grand;
    measure_.increment(MeasureType::Urv2);  // touch prev_
    while (visible->prev_ && !visible->prev_->isVisibleFor(active)) {
      grand = visible->prev_->prev_;  //
      delete visible->prev_;
      measure_.increment(MeasureType::DeleteEpo);
      visible->prev_ = grand;
    }
    return visible->prev_;
  };
#elif GC_MODE == 3
  uint64_t Oat::getMinTs() {
    return actives_.actives_[actives_.actives_.size() - 1];
  }
#endif
};