#include "dvector.h"
#include <cassert>

DVector::DVector(size_t d) : dim(d), vals(dim) {}

const size_t &DVector::dimension() const {
  assert(dim == vals.size());
  return dim;
}

double &DVector::operator[](size_t i) {
  assert(i < dimension());
  return vals[i];
}

const double &DVector::operator[](size_t i) const {
  assert(i < dimension());
  return vals[i];
}
