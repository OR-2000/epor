#pragma once

#include <cstdint>

class Request {
 public:
  enum class Operation { READ, WRITE };  // TODO
  Operation getOperation() const { return operation_; };
  void setOperation(Operation ope) { operation_ = ope; };
  uint64_t getIndex() const { return index_; };
  void setIndex(uint64_t idx) { index_ = idx; }

 private:
  Operation operation_;  // TODO
  uint64_t index_;       // TODO
};