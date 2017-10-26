#include "simplex.h"

class Pricer {
public:
  Simplex::PricingResult
  price(const DVector &x, const LP &lp, const DVector &d,
        const std::vector<size_t> &non_basic_indices) const;
};
