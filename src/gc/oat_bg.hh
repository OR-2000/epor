#pragma once

#include <queue>

#include "../db/record.hh"
#include "../db/version.hh"
#include "../operations.hh"
#include "oat.hh"

class CommitVersion {
 public:
  CommitVersion(Record *, Version *, uint64_t);
  Record *record_;
  Version *version_;
  uint64_t commit_ts_;
};

class OatBg : public Oat {
 public:
  OatBg(const int, Measure &);
  void reclaim(Operations &, uint64_t);

 private:
  std::queue<CommitVersion> writes_;
};