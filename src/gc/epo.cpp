#if GC_MODE == 1 || GC_MODE == 4
#include "epo.hh"

#include <cassert>

#if GC_MODE == 4
Epo::Epo(Oat &oat) : oat_(oat){};
#else
Epo::Epo(const int id, Measure &masure) : Oat(id, masure){};
#endif

#if GC_MODE == 4
void Epo::reclaim(Version *visible) {  // latest_
  oat_.updateLocalActives();
  
  visible = retrieveFirstVisible(visible);
  if (!visible) return;  // using obsolete local actives
  // reclaim in-between version
  for (int idx = 1; idx < oat_.actives_.actives_.size(); idx++) {
    if (!visible->isVisibleFor(oat_.actives_.actives_[idx])) {
      visible = oat_.retrieveNextVisible(oat_.actives_.actives_[idx], visible);
    }
    if (!visible) return;  // using obsolete local actives
  };
  // reclaim by oldest active transaction
  oat_.pruning(visible);
};
#else
void Epo::reclaim(Version *visible) {  // latest_
  updateLocalActives();
  
  visible = retrieveFirstVisible(visible);
  if (!visible) return;  // using obsolete local actives
  // reclaim in-between version
  for (int idx = 1; idx < actives_.actives_.size(); idx++) {
    if (!visible->isVisibleFor(actives_.actives_[idx])) {
      visible = retrieveNextVisible(actives_.actives_[idx], visible);
    }
    if (!visible) return;  // using obsolete local actives
  };
  // reclaim by oldest active transaction
  pruning(visible);
};
#endif

#endif