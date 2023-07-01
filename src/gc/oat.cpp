#include "oat.hh"

#include "../global/actives.hh"
#include "../measure/measure_type.hh"

Oat::Oat(const int id, Measure &measure)
    : measure_(measure), active_(global::actives[id]){};
