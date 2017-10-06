#ifndef DVECTOR_H
#define DVECTOR_H

#include "svector.h"
#include <iostream>
#include <valarray>

class DVector {
public:
  explicit DVector(size_t dimension = 0);
  // cppcheck-suppress noExplicitConstructor
  DVector(std::initializer_list<double> l);

  const size_t &dimension() const;
  void resize(size_t new_dim);
  void append(double val);
  bool operator==(const DVector &rhs) const;
  bool operator!=(const DVector &rhs) const;
  double &operator[](size_t i);
  const double &operator[](size_t i) const;

  double operator*(const DVector &rhs) const;
  double operator*(const SVector &rhs) const;
  DVector &operator-=(const DVector &rhs);
  DVector &operator-=(const SVector &rhs);

private:
  std::valarray<double> vals;
  size_t dim;
};

std::ostream &operator<<(std::ostream &os, const DVector &v);
DVector operator*(const double c, const DVector &rhs);

#endif
