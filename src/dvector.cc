#include "dvector.h"
#include "numeric.h"
#include <cassert>
#include <iostream>

DVector::DVector(size_t d) : vals(d), dim(d) {}

const size_t &DVector::dimension() const {
  assert(dim == vals.size());
  return dim;
}

// cppcheck-suppress passedByValue
DVector::DVector(std::initializer_list<double> l) : vals(l), dim(l.size()) {}

bool DVector::operator==(const DVector &rhs) const {
  assert(dimension() == rhs.dimension());
  for (size_t i = 0; i < dimension(); i++) {
    if (!is_eq(vals[i], rhs[i]))
      return false;
  }
  return true;
}

bool DVector::operator!=(const DVector &rhs) const { return !(*this == rhs); }

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

double DVector::operator*(const SVector &rhs) const {
  double d = 0;
  for (auto it = rhs.cbegin(); it != rhs.cend(); it++)
    d += it->value * vals[it->index];
  return d;
}

std::ostream &operator<<(std::ostream &os, const DVector &v) {
  for (size_t i = 0; i < v.dimension(); i++) {
    if (i > 0)
      os << " ";
    os << v[i];
  }
  return os;
}

DVector operator*(const double c, const DVector &vec) {
  size_t dim = vec.dimension();
  DVector result(dim);
  for (size_t i = 0; i < dim; i++)
    result[i] = c * vec[i];
  return result;
}

DVector &DVector::operator-=(const DVector &rhs) {
  assert(dim == rhs.dimension());
  for (size_t i = 0; i < dim; i++)
    vals[i] -= rhs[i];
  return *this;
}

DVector &DVector::operator-=(const SVector &rhs) {
  for (auto it = rhs.cbegin(); it != rhs.cend(); it++)
    vals[it->index] -= it->value;
  return *this;
}
