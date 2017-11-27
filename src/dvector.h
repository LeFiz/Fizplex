#ifndef DVECTOR_H
#define DVECTOR_H

#include "svector.h"
#include <iostream>
#include <valarray>

class DVector {
public:
  explicit DVector(size_t dimension = 0);
  DVector(std::initializer_list<double> l);

  // Member access
  double &operator[](size_t i);
  const double &operator[](size_t i) const;

  // Size and size manipulation
  void append(double val);
  void resize(size_t new_dim);
  const size_t &dimension() const;

  // Comparison
  double max_abs() const;
  bool operator==(const DVector &rhs) const;
  bool operator!=(const DVector &rhs) const;

  // Iterator access
  auto begin() { return std::begin(vals); }
  auto end() { return std::end(vals); }
  auto cbegin() const { return std::cbegin(vals); }
  auto cend() const { return std::cend(vals); }

  // Math operators
  double operator*(const DVector &rhs) const;
  double operator*(const SVector &rhs) const;
  DVector &operator-=(const DVector &rhs);
  DVector &operator-=(const SVector &rhs);

private:
  std::valarray<double> vals;
  size_t dim;
};

DVector operator*(const double c, const DVector &rhs);

std::ostream &operator<<(std::ostream &os, const DVector &v);

#endif
