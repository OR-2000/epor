#include "tx.hh"

#include <iostream>

#include "../global/records.hh"
#include "../global/timestamp.hh"
#include "../measure/measure_type.hh"
#include "../operation.hh"

Tx::Tx(const int id, Measure &measure)
    : id_(id), db_(id, measure), measure_(measure){};

void Tx::begin() {
  begin_ts_ = global::ts->fetchAdd();
#if GC_MODE != 0
  db_.begin(begin_ts_);  // TODO: EPO, EPO-R, OAT時のみ
#endif
};

uint64_t Tx::read(const uint64_t index) {
  Operation ope(index);
  // avoid: r(x)...r(x) and w(x)...r(x)
  if (read_set_.findForRead(ope) || write_set_.findForRead(ope))
    return ope.value_;
  measure_.increment(MeasureType::Read);
  ope.record_ = global::records->records_[index];
  db_.read(ope, begin_ts_);
  read_set_.add(ope);
  return ope.value_;
};

bool Tx::write(const uint64_t index, const uint64_t value) {
  Operation ope(index, value);
  if (write_set_.findForWrite(ope)) return true;  // avoid: w(x)...w(x)
  measure_.increment(MeasureType::Write);
  // r(x)...w(x)のとき、r(x)からレコードの情報を得ることで不要なレコードアクセスを防ぐ
  if (!read_set_.findForWrite(ope))
    ope.record_ = global::records->records_[index];
  db_.write(ope, begin_ts_);
  if (!ope.version_) return false;
  write_set_.add(ope);
  return true;
};

void Tx::commit() {
  measure_.increment(MeasureType::Commit);
  db_.commit(write_set_);
  reset();
};

void Tx::abort() {
  measure_.increment(MeasureType::Abort);
  db_.abort(write_set_);
  reset();
};
