#if GC_MODE == 2 || GC_MODE == 4

#include "epo_r.hh"

#include <cassert>

#if GC_MODE == 4
EpoR::EpoR(Oat &oat) : oat_(oat){};
#else
EpoR::EpoR(const int id, Measure &measure) : Oat(id, measure){};
#endif

#if GC_MODE == 4
Version *EpoR::reclaim(Version *visible, const uint64_t ts) {  // latest_
  oat_.updateLocalActives();

  Version *read = visible->isVisibleFor(ts) ? visible : nullptr;
  visible = retrieveFirstVisible(visible, ts, read);
  if (!visible) return read;  // using obsolete local actives
  // reclaim in-between version
  for (int idx = 1; idx < oat_.actives_.actives_.size(); idx++) {
    if (!visible->isVisibleFor(oat_.actives_.actives_[idx])) {
      visible = oat_.retrieveNextVisible(oat_.actives_.actives_[idx], visible);
      if (!visible) return read;  // using obsolete local actives
      if (read == nullptr && visible->isVisibleFor(ts)) read = visible;
    }
  };
  // reclaim by old active transaction
  oat_.pruning(visible);
  return read;
};
#else

Version *EpoR::reclaim(Version *visible, const uint64_t ts) {  // latest_
  updateLocalActives();

  Version *read = visible->isVisibleFor(ts) ? visible : nullptr;
  visible = retrieveFirstVisible(visible, ts, read);
  if (!visible) return read;  // using obsolete local actives
  // reclaim in-between version
  for (int idx = 1; idx < actives_.actives_.size(); idx++) {
    if (!visible->isVisibleFor(actives_.actives_[idx])) {
      visible = retrieveNextVisible(actives_.actives_[idx], visible);
      if (!visible) return read;  // using obsolete local actives
      if (read == nullptr && visible->isVisibleFor(ts)) read = visible;
    }
  };
  // reclaim by old active transaction
  pruning(visible);
  return read;
};
#endif

#endif