#pragma once

#include "../db/version.hh"
#include "../definition.hh"
#include "local_actives.hh"
#include "oat.hh"
#include "time_stamp_counter.hh"

#if GC_MODE == 4
class EpoR {
#else
class EpoR : public Oat {
#endif
 public:
#if GC_MODE == 4
  EpoR(Oat &oat);
#else
  EpoR(const int, Measure &);
#endif
  Version *reclaim(Version *, const uint64_t);

 private:
#if GC_MODE == 4
  Version *retrieveFirstVisible(Version *visible, const uint64_t ts,
                                Version *&read) {  // committed or pending
    while (visible && !visible->isVisibleFor(oat_.actives_.actives_[0])) {
      visible = oat_.retrieveNextComitted(visible);
      if (visible && read == nullptr && visible->isVisibleFor(ts)) read = visible;
    }
    return visible;
  };
#else
  Version *retrieveFirstVisible(Version *visible, const uint64_t ts,
                                Version *&read) {  // committed or pending
    while (visible && !visible->isVisibleFor(actives_.actives_[0])) {
      visible = retrieveNextComitted(visible);
      if (visible && read == nullptr && visible->isVisibleFor(ts)) read = visible;
    }
    return visible;
  };
#endif

#if GC_MODE == 4
  Oat &oat_;
#endif
};