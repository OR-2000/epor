#include "requests.hh"

#include <cassert>

#include "../definition.hh"
#include "../gflags.hh"

Requests::Util::Util() : rnd_(), zipf_(&rnd_, FLAGS_skew, FLAGS_record_size){};

void Requests::generate() {
  for (int i = 0; i < request_size_; i++) {
    reqs_[i].setIndex(util_.zipf_());
    if (util_.rnd_.next() % 100 < read_ratio_) {
      reqs_[i].setOperation(Request::Operation::READ);
    } else {
      reqs_[i].setOperation(Request::Operation::WRITE);
    }
  }
};

Requests::Requests(const int id) {
  if (FLAGS_read_ratio_long) {
    if (IS_LONG(id)) {  // TODO
      read_ratio_ = FLAGS_read_ratio_long;
    } else {
      read_ratio_ = FLAGS_read_ratio;
    }
  } else {
    read_ratio_ = FLAGS_read_ratio;
  }

  if (FLAGS_request_size_long) {
    if (IS_LONG(id)) {  // TODO
      reqs_ = new Request[FLAGS_request_size_long];
      request_size_ = FLAGS_request_size_long;
    } else {
      reqs_ = new Request[FLAGS_request_size];
      request_size_ = FLAGS_request_size;
    }
  } else {
    reqs_ = new Request[FLAGS_request_size];
    request_size_ = FLAGS_request_size;
  }
}
Requests::~Requests() { delete[] reqs_; }
