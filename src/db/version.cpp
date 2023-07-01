#include "version.hh"

#include <cassert>
#include <iostream>

Version::Version()
    : prev_(nullptr), value_(0), status_(Status::COMMITTED), commit_ts_(0){};
Version::Version(Version *prev, const uint64_t value)
    : prev_(prev), value_(value), status_(Status::PENDING), commit_ts_(0){};
