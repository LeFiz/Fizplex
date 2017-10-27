#include "simplex.h"

class RatioTester {
public:
  Simplex::RatioTestResult ratio_test(const LP &lp, const SVector &alpha,
                                      const DVector &beta,
                                      const size_t candidate_index,
                                      const std::vector<size_t> &basic_indices,
                                      const double candidate_cost) const;
};
