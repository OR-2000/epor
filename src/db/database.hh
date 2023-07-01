#pragma once

#include "../measure/measure.hh"
#include "../operation.hh"
#include "../operations.hh"

#if GC_MODE == 1
#include "../gc/epo.hh"
#elif GC_MODE == 2
#include "../gc/epo_r.hh"
#elif GC_MODE == 3
#include "../gc/oat_bg.hh"
#elif GC_MODE == 4
#include "../gc/oat.hh"
#include "../gc/epo.hh"
#include "../gc/epo_r.hh"
#endif

class Database {
 public:
  Database(const int, Measure &);
#if GC_MODE == 4
  void begin(const uint64_t ts) { oat_.begin(ts); };
#elif GC_MODE != 0
  void begin(const uint64_t ts) { gc_.begin(ts); };
#endif
  void read(Operation &, const uint64_t);
  void write(Operation &, const uint64_t);
  void commit(Operations &);
  void abort(Operations &);

#if GC_MODE == 2
  Version *findVisible(Version *&latest, const uint64_t ts) {
    gc_.makeLatestNotAborted(latest);
    return gc_.reclaim(latest, ts);
  };
#elif GC_MODE == 4
  Version *findVisible(Record *record, const uint64_t ts) {
    if(FLAGS_tpr_limit <= record->tpr_)  {
      record->tpr_ = 0;
      oat_.makeLatestNotAborted(record->latest_);
      return epor_.reclaim(record->latest_, ts);
    }

    Version *v = record->latest_;
    measure_.increment(MeasureType::RTouch);  // touch latest_
    while (!v->isVisibleFor(ts)) {
      v = v->prev_;
      measure_.increment(MeasureType::RTouch);  // touch prev_
    };
    return v;
  };
#else
  Version *findVisible(Version *v, const uint64_t ts) {
    measure_.increment(MeasureType::RTouch);  // touch latest_
    while (!v->isVisibleFor(ts)) {
      v = v->prev_;
      measure_.increment(MeasureType::RTouch);  // touch prev_
    };
    return v;
  };
#endif

#if GC_MODE == 1
  // Pending: 相手がアボートするかもしれないけど待たずに自分をアボート
  bool existConcurrentUpdater(Version *&latest, const uint64_t ts) {
    gc_.makeLatestNotAborted(latest);
    gc_.reclaim(latest);
    return latest->isPending() || latest->isTooLate(ts);
  };
#elif GC_MODE == 4
  // Pending: 相手がアボートするかもしれないけど待たずに自分をアボート
  bool existConcurrentUpdater(Record *record, const uint64_t ts) {
    if(FLAGS_tpr_limit <= record->tpr_)  {
      record->tpr_ = 0;
      oat_.makeLatestNotAborted(record->latest_);
      epo_.reclaim(record->latest_);
    }

    return record->latest_->isPending() || record->latest_->isTooLate(ts);
  };
#else
  // Pending: 相手がアボートするかもしれないけど待たずに自分をアボート
  bool existConcurrentUpdater(Version *v, const uint64_t ts) { // latest
    while(v->isAborted()) {
      v = v->prev_;
    };
    return v->isPending() || v->isTooLate(ts);
  };
#endif


 private:
#if GC_MODE == 1
  Epo gc_;
#elif GC_MODE == 2
  EpoR gc_;
#elif GC_MODE == 3
  OatBg gc_;
#elif GC_MODE == 4
  Oat oat_;
  Epo epo_;
  EpoR epor_;
#endif
  Measure &measure_;
};