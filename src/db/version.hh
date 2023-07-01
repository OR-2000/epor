#pragma once

#include <cstdint>

#include "../definition.hh"

class CACHE_ALIGNED Version {
 public:
  Version();
  Version(Version *, const uint64_t);

  bool isTooLate(const uint64_t begin_ts) { return begin_ts < commit_ts_; }
  bool isVisibleFor(const uint64_t begin_ts) {
    return status_ == Status::COMMITTED && commit_ts_ < begin_ts;
  }
  bool isAborted() { return status_ == Status::ABORTED; }
  bool isPending() { return status_ == Status::PENDING; }
  bool isCommitted() { return status_ == Status::COMMITTED; }

  void commit(const uint64_t commit_ts) {
    commit_ts_ = commit_ts;
    status_ = Status::COMMITTED;
  };
  void abort() { status_ = Status::ABORTED; };

  Version *prev_;  // TODO

  //  private: TODO 削除
  enum Status : unsigned int { ABORTED, COMMITTED, PENDING };

  uint64_t value_;
  Status status_;

  uint64_t commit_ts_;
};