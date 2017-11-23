#include "simplex.h"
#include "base.h"
#include "debug.h"
#include "pricer.h"
#include "ratio_tester.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

Simplex::Simplex(LP &_lp)
    : lp(_lp), x(lp.A.col_count() + lp.A.row_count()),
      c(lp.A.col_count() + lp.A.row_count()) {
  const size_t structural_count = lp.A.col_count();
  lp.add_logicals();
  for (size_t i = 0; i < structural_count; i++)
    non_basic_indices.push_back(i);
  for (size_t i = structural_count; i < lp.A.col_count(); i++)
    basic_indices.push_back(i);
}

const DVector &Simplex::get_x() const { return x; }

const double &Simplex::get_z() const { return z; }

const Simplex::Result &Simplex::get_result() const { return result; }

void Simplex::set_initial_x() {
  for (size_t i = 0; i < lp.A.col_count(); i++) {
    auto col_header = lp.column_header(i);
    if (is_finite(col_header.lower)) {
      x[i] = col_header.lower;
    } else if (is_finite(col_header.upper)) {
      x[i] = col_header.upper;
    } else {
      x[i] = 0.0;
    }
  }
}

void Simplex::solve() {

  set_initial_x();

  for (int round = 0; round < max_rounds; round++) {
    Base base(lp, basic_indices);
    set_basic_solution(base);

    if (phase == Simplex::Phase::One)
      set_phase_one_objective();
    z = c * x;

    auto candidate = run_price(base);

    IterationDecision iteration_decision = IterationDecision::Unfinished;
    if (candidate.is_optimal) {
      if (phase == Simplex::Phase::Two) {
        assert(lp.is_feasible(x));
        iteration_decision = IterationDecision::OptimalSolution;
      } else { // Phase I
        if (lp.is_feasible(x)) {
          iteration_decision = IterationDecision::SwitchToPhaseTwo;
        } else {
          iteration_decision = IterationDecision::Infeasible;
        }
      }
    }

    const auto rt = run_ratio_test(candidate, base);

    if (iteration_decision == IterationDecision::Unfinished)
      iteration_decision = rt.result;

    print_iteration_results(iteration_decision, round);

    switch (iteration_decision) {
    case IterationDecision::BaseChange: {
      x[basic_indices[rt.leaving_index]] = rt.leaving_bound;
      const auto it = std::find(non_basic_indices.begin(),
                                non_basic_indices.end(), candidate.index);

      assert(it != non_basic_indices.end());
      const size_t candidate_non_basic_index =
          std::distance(non_basic_indices.begin(), it);

      std::swap<size_t>(non_basic_indices[candidate_non_basic_index],
                        basic_indices[rt.leaving_index]);
      break;
    }
    case IterationDecision::Unbounded:
      assert(phase != Simplex::Phase::One);

      result = Result::Unbounded;
      x[basic_indices[rt.leaving_index]] = rt.leaving_bound;
      x[candidate.index] = rt.step_length;
      z = -inf;
      return;
    case IterationDecision::BoundFlip:
      x[candidate.index] += rt.step_length;
      break;
    case IterationDecision::OptimalSolution:
      result = Result::OptimalSolution;
      return;
    case IterationDecision::SwitchToPhaseTwo:
      phase = Simplex::Phase::Two;
      c = lp.c;
      break;
    case IterationDecision::Infeasible:
      result = Result::Infeasible;
      return;
    default:
      assert(false);
    }
  }
}

void Simplex::set_phase_one_objective() {
  for (auto i : non_basic_indices)
    c[i] = 0.0;
  for (auto i : basic_indices) {
    const auto column_header = lp.column_header(i);
    if (is_finite(column_header.lower) &&
        is_lower_norm(x[i], column_header.lower)) {
      c[i] = -1.0;
    } else if (is_finite(column_header.upper) &&
               is_greater_norm(x[i], column_header.upper)) {
      c[i] = 1.0;
    } else
      c[i] = 0.0;
  }
}

void Simplex::set_basic_solution(Base &base) {
  DVector beta = lp.b;
  for (auto i : non_basic_indices)
    beta -= x[i] * lp.A.column(i);
  base.ftran(beta);

  for (size_t i = 0; i < basic_indices.size(); i++)
    x[basic_indices[i]] = beta[i];

  if (phase == Phase::Two)
    assert(lp.is_feasible(x));
}

Simplex::Candidate Simplex::run_price(const Base &base) const {
  DVector pi(lp.A.row_count());
  DVector d(lp.A.col_count());
  for (size_t i = 0; i < lp.A.row_count(); i++)
    pi[i] = c[basic_indices[i]]; // ==c_B
  base.btran(pi);
  for (size_t i = 0; i < lp.A.col_count(); i++)
    d[i] = c[i] - pi * lp.A.column(i);

  return Pricer().price(x, lp, d, non_basic_indices);
}

Simplex::RatioTestResult Simplex::run_ratio_test(Simplex::Candidate candidate,
                                                 Base &base) const {
  SVector alpha = lp.A.column(candidate.index);
  base.ftran(alpha);
  return RatioTester().ratio_test(lp, alpha, x, candidate.index, basic_indices,
                                  candidate.cost);
}

void Simplex::print_iteration_results(IterationDecision &id, int round) const {
  std::cout << round << " | " << phase << " | " << id << " | " << z << "\n";
}

std::ostream &operator<<(std::ostream &os, const Simplex::Phase &p) {
  os << (p == Simplex::Phase::One ? "One" : "Two");
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const Simplex::IterationDecision &id) {
  static std::unordered_map<Simplex::IterationDecision, std::string> idtos = {
      {Simplex::IterationDecision::Unbounded, "Unbounded"},
      {Simplex::IterationDecision::BoundFlip, "BoundFlip"},
      {Simplex::IterationDecision::BaseChange, "BaseChange"},
      {Simplex::IterationDecision::OptimalSolution, "OptimalSolution"},
      {Simplex::IterationDecision::SwitchToPhaseTwo, "SwitchToPhaseTwo"},
      {Simplex::IterationDecision::Infeasible, "Infeasible"},
      {Simplex::IterationDecision::Unfinished, "Unfinished"}};
  os << idtos[id];
  return os;
}
