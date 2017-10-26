#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "lp.h"

class Simplex {
public:
  enum class Result { Infeasible, OptimalSolution, Unbounded, Unsolved };
  struct PricingResult {
    bool is_optimal;
    size_t candidate_index;
  };

  explicit Simplex(LP &_lp);
  void solve();

  const DVector &get_x() const;
  const double &get_z() const;
  const Result &get_result() const;

private:
  enum class Phase { One, Two };
  enum class IterationDecision {
    Unbounded,
    BoundFlip,
    BaseChange,
    OptimalSolution,
    SwitchToPhaseTwo,
    Infeasible,
    Unfinished
  };
  struct RatioTestResult {
    IterationDecision result;
    size_t leaving_index;
    double step_length;
    double leaving_bound;
  };
  LP &lp;
  DVector x;
  double z = inf;
  Result result = Result::Unsolved;
  Phase phase = Phase::One;

  void set_initial_x();
  void print_iteration_results(IterationDecision &, int) const;
  RatioTestResult ratio_test(SVector &alpha, DVector &beta,
                             size_t candidate_index,
                             std::vector<size_t> &basic_indices,
                             double candidate_cost) const;
  static constexpr int max_rounds = 999;

  friend std::ostream &operator<<(std::ostream &, const Simplex::Phase &);
  friend std::ostream &operator<<(std::ostream &,
                                  const Simplex::IterationDecision &);
};

#endif
