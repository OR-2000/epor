#pragma once

#include <thread>

#include "../db/access_counter.hh"
#include "rendezvous.hh"

class MultiClient {
 public:
  MultiClient();
  ~MultiClient();

 private:
  static void request(const int, Rendezvous &, AccessCounter &);
  AccessCounter ac_;  // for debug
  std::thread **threads_;
  Rendezvous rendezvous_;
  void join();
  void sleep();
};