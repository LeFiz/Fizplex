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

double DVector::operator*(const DVector &rhs) const {
  assert(dimension() == rhs.dimension());
  double d = 0;
  for (size_t i = 0; i < dimension(); i++)
    d += vals[i] * rhs[i];
  return d;
}
