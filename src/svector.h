#ifndef SVECTOR_H
#define SVECTOR_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "numeric.h"

namespace fizplex {

// Sparse Vector
class SVector {
public:
  SVector();
  SVector(std::initializer_list<Nonzero>);

  // Member access
  void add_value(size_t ind, double val);
  double get_value(size_t ind) const;
  double &operator[](size_t);
  size_t length() const;

  // Iterator access
  auto begin() { return values.begin(); }
  auto end() { return values.end(); }
  auto cbegin() const { return values.cbegin(); }
  auto cend() const { return values.cend(); }

  // Comparison operators
  bool operator==(const SVector &rhs) const;
  bool operator!=(const SVector &rhs) const;

private:
  std::vector<Nonzero> values;
};

SVector operator*(const double c, const SVector &vec);

std::ostream &operator<<(std::ostream &os, Nonzero const &n);
std::ostream &operator<<(std::ostream &os, SVector const &n);

} // namespace fizplex

#endif
