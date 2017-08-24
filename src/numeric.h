#ifndef NUMERIC_H
#define NUMERIC_H

#include <cmath>
#include <cstddef>
#include <limits>

const double eps = 1e-9;

const double inf = std::numeric_limits<double>::infinity();
const double neg_inf = (-1) * std::numeric_limits<double>::infinity();

inline bool is_eq(double lhs, double rhs) { return fabs(lhs - rhs) < eps; }

inline bool is_zero(double val) { return fabs(val) < eps; }

struct Nonzero {
  size_t index;
  double value;
};

#endif
