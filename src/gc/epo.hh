
#pragma once

#include "../db/version.hh"
#include "../definition.hh"
#include "oat.hh"

#if GC_MODE == 4
class Epo {
#else
class Epo : public Oat {
#endif
 public:
#if GC_MODE == 4
  Epo(Oat &oat);
#else
  Epo(const int, Measure &);
#endif
  void reclaim(Version *);

 private:
#if GC_MODE == 4
  Version *retrieveFirstVisible(Version *v) {  // Committed or Pending
    while (v && !v->isVisibleFor(oat_.actives_.actives_[0])) {
      v = oat_.retrieveNextComitted(v);
    }
    return v;
  };
#else
  Version *retrieveFirstVisible(Version *v) {  // Committed or Pending
    while (v && !v->isVisibleFor(actives_.actives_[0])) {
      v = retrieveNextComitted(v);
    }
    return v;
  };
#endif
#if GC_MODE == 4
  Oat &oat_;
#endif
};