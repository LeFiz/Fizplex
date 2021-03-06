#ifndef NUMERIC_H
#define NUMERIC_H

#include <cmath>
#include <cstddef>
#include <limits>

namespace fizplex {

constexpr double eps = 1e-6;
constexpr double zero_eps = 1e-7;
constexpr double inf = std::numeric_limits<double>::infinity();

inline bool is_eq_norm(double lhs, double rhs, double epsilon = eps,
                       double zero_epsilon = zero_eps) {
  const auto diff = fabs(lhs - rhs);
  if (diff <= zero_epsilon)
    return true;
  const auto l = fabs(lhs);
  const auto r = fabs(rhs);
  const auto larger = l > r ? l : r;
  if (diff <= epsilon * larger)
    return true;
  return false;
}

inline bool is_lower_norm(double lhs, double rhs) {
  return (lhs < rhs) && !is_eq_norm(lhs, rhs);
}
inline bool is_greater_norm(double lhs, double rhs) {
  return (lhs > rhs) && !is_eq_norm(lhs, rhs);
}

inline bool is_zero(double val, double epsilon = zero_eps) {
  return fabs(val) <= epsilon;
}
inline bool is_ge(double lhs, double rhs) { return (lhs + eps) >= rhs; }
inline bool is_le(double lhs, double rhs) { return (rhs + eps) >= lhs; }
inline bool is_lower(double lhs, double rhs) { return (lhs + eps) < rhs; }
inline bool is_greater(double lhs, double rhs) { return (rhs + eps) < lhs; }

inline bool is_finite(double val) { return std::isfinite(val); }
inline bool is_infinite(double val) { return !std::isfinite(val); }

} // namespace fizplex

#endif
