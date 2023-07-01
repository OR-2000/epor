#pragma once

#include "../operation.hh"
#include "../operations.hh"

class Records {
 public:
  Records();
  ~Records();
  Record **records_;
};