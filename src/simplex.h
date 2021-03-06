#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "base.h"
#include "lp.h"

namespace fizplex {

class Simplex {
public:
  enum class Result { Infeasible, OptimalSolution, Unbounded, Unsolved };
  struct Candidate {
    bool is_optimal;
    size_t index;
    double cost;
  };
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

  explicit Simplex(LP &_lp, int print_level_ = 0);
  void solve();

  const DVector &get_x() const;
  const double &get_z() const;
  const Result &get_result() const;

private:
  enum class Phase { One, Two };
  LP &lp;
  int print_level;
  std::vector<size_t> basic_indices;
  std::vector<size_t> non_basic_indices;
  DVector x;
  DVector c;
  double z = inf;
  Result result = Result::Unsolved;
  Phase phase = Phase::One;

  void set_initial_x();
  void set_phase_one_objective();
  void set_basic_solution(Base &);
  Candidate run_price(const Base &) const;
  RatioTestResult run_ratio_test(Candidate, Base &) const;
  IterationDecision decision_for_optimality() const;
  void exchange_base_column(Candidate, RatioTestResult);
  void set_result_for_unbounded(Candidate, RatioTestResult);

  void print_iteration_results(IterationDecision &, int) const;
  static constexpr int max_rounds = 5000;

  friend std::ostream &operator<<(std::ostream &, const Simplex::Phase &);
  friend std::ostream &operator<<(std::ostream &,
                                  const Simplex::IterationDecision &);
};

} // namespace fizplex

#endif
