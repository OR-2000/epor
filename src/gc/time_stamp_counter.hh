#pragma once

#include <cstdint>

#include "../definition.hh"

class TimeStampCounter {
 private:
  uint64_t lap_;
  uint64_t rdtsc() {
    uint32_t EDX, EAX;
    asm volatile("rdtsc" : "=d"(EDX), "=a"(EAX)::);
    return ((uint64_t)EDX << 32) | EAX;
  }

 public:
  uint64_t getElapsedMsFromLap() { return (rdtsc() - lap_) / CLOCKS_PER_MS; };
  uint64_t getElapsedSecFromLap() { return (rdtsc() - lap_) / CLOCKS_PER_S; };
  uint64_t getElapsedUsFromLap() { return (rdtsc() - lap_) / CLOCKS_PER_US; };
  TimeStampCounter();
  void lap() { lap_ = rdtsc(); };
};