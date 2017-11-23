#include "ratio_tester.h"

Simplex::RatioTestResult
RatioTester::ratio_test(const LP &lp, const SVector &alpha, const DVector &x,
                        const size_t candidate_index,
                        const std::vector<size_t> &basic_indices,
                        const double candidate_cost) const {
  double min_theta = inf;
  size_t min_theta_posi = 0;
  double bound = inf;
  double leaving_bound = inf;
  double a = inf;
  const double direction = is_ge(candidate_cost, 0.0) ? -1.0 : 1.0;

  for (auto n = alpha.cbegin(); n != alpha.cend(); ++n) {
    if (is_zero(n->value))
      continue; // alpha_i = 0 can not limit displacement

    const auto &column_header = lp.column_header(basic_indices[n->index]);
    if (column_header.type == ColType::Fixed) {
      min_theta = 0; // Fixed variable always means step_length = 0
      min_theta_posi = n->index;
      leaving_bound = column_header.lower;
      break;
    }

    a = n->value * direction;
    if (a < 0.0) {
      bound = column_header.upper; // = inf for ColType = Lower
    } else {
      bound = column_header.lower; // = -inf for ColType = upper
    }
    double t = (x[basic_indices[n->index]] - bound) / a;
    if (t < min_theta) {
      min_theta = t;
      min_theta_posi = n->index;
      leaving_bound = bound;
    }
  }

  const double max_steplength = lp.column_header(candidate_index).upper -
                                lp.column_header(candidate_index).lower;
  min_theta = std::min<double>(min_theta, max_steplength);

  Simplex::RatioTestResult rt;
  rt.step_length = direction * min_theta; // make signed again
  rt.leaving_bound = leaving_bound;
  if (is_infinite(min_theta)) {
    rt.result = Simplex::IterationDecision::Unbounded;
    rt.leaving_index = min_theta_posi;
  } else if (is_finite(max_steplength) &&
             is_eq_norm(min_theta, max_steplength)) {
    rt.result = Simplex::IterationDecision::BoundFlip;
  } else {
    rt.result = Simplex::IterationDecision::BaseChange;
    rt.leaving_index = min_theta_posi;
  }
  return rt;
}
