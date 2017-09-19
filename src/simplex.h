#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "lp.h"

class Simplex {
public:
  enum class Result { Infeasible, OptimalSolution, Unbounded, Unsolved };

  explicit Simplex(const LP &_lp);
  void solve();

  const DVector &get_x() const;
  const double &get_z() const;
  const Result &get_result() const;

  bool print_iterations;

private:
  struct PricingResult {
    bool is_optimal;
    size_t candidate_index;
  };
  enum class IterationResult { Unbounded, BoundFlip, BaseChange };
  enum class Phase { One, Two };
  struct RatioTestResult {
    IterationResult result;
    size_t leaving_index;
    double step_length;
    double leaving_bound;
  };
  const LP &lp;
  DVector x;
  double z;
  Result result;
  size_t structural_count;
  size_t row_count;
  size_t col_count;
  Phase phase;

  void set_initial_x();
  PricingResult price(DVector &pi,
                      std::vector<size_t> &non_basic_indices) const;
  RatioTestResult ratio_test(SVector &alpha, DVector &beta,
                             size_t candidate_index,
                             std::vector<size_t> &basic_indices,
                             double candidate_cost) const;
};

#endif
