#include "database.hh"

#include <cassert>

#include "../global/timestamp.hh"
#include "../measure/measure_type.hh"
#include "lock.hh"
#include "version.hh"

#if GC_MODE == 0
Database::Database(const int id, Measure &measure) : measure_(measure){};
#elif GC_MODE == 4
Database::Database(const int id, Measure &measure) : measure_(measure), oat_(id, measure), epo_(oat_), epor_(oat_) {};
#else
Database::Database(const int id, Measure &measure)
    : gc_(id, measure), measure_(measure){};
#endif

void Database::read(Operation &ope, const uint64_t ts) {
  // critical section
  Lock lock(ope.record_->counter_);
#if GC_MODE == 4
  ope.version_ = findVisible(ope.record_, ts);
#else
  ope.version_ = findVisible(ope.record_->latest_, ts);
#endif
  ope.value_ = ope.version_->value_;
};

void Database::write(Operation &ope, const uint64_t ts) {
  // critical section
  Lock lock(ope.record_->counter_);
#if GC_MODE == 4
  if (existConcurrentUpdater(ope.record_, ts)) return;
#else
  if (existConcurrentUpdater(ope.record_->latest_, ts)) return;
#endif
  // append new version
  ope.version_ = new Version(ope.record_->latest_, ope.value_);
  measure_.increment(MeasureType::Create);
  ope.record_->latest_ = ope.version_;
#if GC_MODE == 4
  ope.record_->tpr_++;
#endif
}

void Database::commit(Operations &writes) {
  if (writes.ops_.size() == 0) return;  // TODO
  uint64_t commit_ts;
  writes.sort();
  {  // critical section
    Lock *locks[writes.ops_.size()];
    for (int i = 0; i < writes.ops_.size(); i++) {
      locks[i] = new Lock(writes.ops_[i].record_->counter_);
    }
    commit_ts = global::ts->fetchAdd();
    writes.commit(commit_ts);
    for (int i = 0; i < writes.ops_.size(); i++) {
      delete locks[i];
    }
  }  // critical section
#if GC_MODE == 3
  gc_.reclaim(writes, commit_ts);
#endif
};

void Database::abort(Operations &writes) { writes.abort(); };
