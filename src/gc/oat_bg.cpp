#if GC_MODE == 3
#include "oat_bg.hh"

#include "../db/lock.hh"

OatBg::OatBg(const int id, Measure &measure) : Oat(id, measure) {};

CommitVersion::CommitVersion(Record* record, Version* version, uint64_t ts)
    : record_(record), version_(version), commit_ts_(ts){};

void OatBg::reclaim(Operations& writes, uint64_t commit_ts) {
  for (int i = 0; i < writes.ops_.size(); i++) {
    writes_.push(CommitVersion(writes.ops_[i].record_, writes.ops_[i].version_, commit_ts));
  }
  if (!updateLocalActives()) return;
  uint64_t minTs = getMinTs();
  while (!writes_.empty()) {
    if (minTs <= writes_.front().commit_ts_) break;
    {
      Lock lock(writes_.front().record_->counter_);
      // gc_ts_未満のバージョンはすでに回収済み
      if (writes_.front().commit_ts_ < writes_.front().record_->gc_ts_) {
        writes_.pop();
        continue;
      }
      pruning(writes_.front().version_);
      writes_.front().record_->gc_ts_ = writes_.front().commit_ts_;
    }                                      // critical section
    writes_.pop();
  }
}
#endif