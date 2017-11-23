#include "pricer.h"
#include "simplex.h"

Simplex::PricingResult
Pricer::price(const DVector &x, const LP &lp, const DVector &d,
              const std::vector<size_t> &non_basic_indices) const {
  double min_val = 0.0;
  size_t min_posi = 0;
  bool updated = false;
  for (auto j : non_basic_indices) {
    const auto ch = lp.column_header(j);
    if (ch.type == ColType::Fixed)
      continue; // Fixed vars should not enter the basis

    double sign = 1.0;
    if ((is_finite(ch.upper) && is_eq_norm(x[j], ch.upper)) ||
        (ch.type == ColType::Free && is_greater(d[j], 0.0)))
      sign = -1.0;

    if (is_lower_norm(sign * d[j], min_val)) {
      min_val = sign * d[j];
      min_posi = j;
      updated = true;
    }
  }
  Simplex::PricingResult pr;
  pr.is_optimal = !updated;
  pr.candidate_index = min_posi;
  pr.candidate_cost = d[pr.candidate_index];
  return pr;
}
