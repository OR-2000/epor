#pragma once

#include "../definition.hh"

class Rendezvous {
 public:
  bool isContinuable() { return !__atomic_load_n(&end_, __ATOMIC_SEQ_CST); };
  bool isStarted();
  void notifyReady();
  void waitStart();

  void rendezvous();
  void announceTheStart();
  void announceTheEnd();

  Rendezvous();

 private:
  CACHE_ALIGNED int busy_;
  CACHE_ALIGNED bool start_;
  CACHE_ALIGNED bool end_;
};