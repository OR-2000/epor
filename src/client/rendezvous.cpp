#include "rendezvous.hh"

#include "../definition.hh"
#include "../gflags.hh"

Rendezvous::Rendezvous()
    : busy_(FLAGS_thread_size), start_(false), end_(false){};

bool Rendezvous::isStarted() {
  return __atomic_load_n(&start_, __ATOMIC_SEQ_CST);
};
void Rendezvous::waitStart() {
  while (!isStarted()) {
    cpu_relax();
  }
};
void Rendezvous::notifyReady() {
  __atomic_fetch_sub(&busy_, 1, __ATOMIC_SEQ_CST);
};

void Rendezvous::rendezvous() {
  while (0 < __atomic_load_n(&busy_, __ATOMIC_SEQ_CST)) {
    cpu_relax();
  }
};
void Rendezvous::announceTheStart() {
  __atomic_store_n(&start_, true, __ATOMIC_SEQ_CST);
};
void Rendezvous::announceTheEnd() {
  __atomic_store_n(&end_, true, __ATOMIC_SEQ_CST);
};
