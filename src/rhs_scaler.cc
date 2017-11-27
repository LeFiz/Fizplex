#include "rhs_scaler.h"
#include <cassert>

namespace fizplex {

RhsScaler::RhsScaler(double s) : scale_factor(s) { assert(scale_factor > 0.0); }

void RhsScaler::scale(double &d) const {
  if (scale_factor > 0.0)
    d /= scale_factor;
}

void RhsScaler::scale(DVector &vec) const {
  if (scale_factor > 0.0)
    vec = (1 / scale_factor) * vec;
}

void RhsScaler::unscale(double &d) const {
  if (scale_factor > 0.0)
    d *= scale_factor;
}

void RhsScaler::unscale(DVector &vec) const {
  if (scale_factor > 0.0)
    vec = scale_factor * vec;
}

} // namespace fizplex
