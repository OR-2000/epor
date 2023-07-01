#pragma once

#include "../include/xoroshiro128plus.hh"
#include "../include/zipf.hh"
#include "request.hh"

class Requests {
 private:
  class Util {
   public:
    Util();
    Xoroshiro128Plus rnd_;
    FastZipf zipf_;
  };
  Util util_;
  int read_ratio_;
  int request_size_;
  Request *reqs_;

 public:
  void generate();
  Requests(const int);
  ~Requests();
  int size() { return request_size_; };
  Request &operator[](int i) { return reqs_[i]; }
};
