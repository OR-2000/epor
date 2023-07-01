#include "operations.hh"


bool Operations::findForRead(Operation& target) {
  for (Operation& op : ops_) {
    if (target.index_ == op.index_) {
      target.value_ = op.value_;
      return true;
    }
  };
  return false;
};

bool Operations::findForWrite(Operation& target) {
  for (Operation& op : ops_) {
    if (target.index_ == op.index_) {
      target.record_ = op.record_;
      return true;
    }
  };
  return false;
};
