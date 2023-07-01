#pragma once

#include <algorithm>

#include <vector>

#include "db/lock.hh"
#include "operation.hh"

class Operations {
 public:
  std::vector<Operation> ops_;

  bool findForRead(Operation &);
  bool findForWrite(Operation &);
  void add(Operation &op) {
    ops_.emplace_back(op.record_, op.index_, op.value_, op.version_);
  }
  void clear() { ops_.clear(); }

  void sort() {
    std::sort(ops_.begin(), ops_.end(),
              [](const Operation &x, const Operation &y) {
                return x.record_ < y.record_;
              });
  }
  void commit(const uint64_t commit_ts) {
    for (int i = 0; i < ops_.size(); i++) {
      ops_[i].version_->commit(commit_ts);
    }
  }
  void abort() {
    for (int i = 0; i < ops_.size(); i++) {
      Lock lock(ops_[i].record_->counter_);
      ops_[i].version_->abort();
    }
  };
};
