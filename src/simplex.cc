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
  const size_t col_count = lp.A.col_count();
  const size_t row_count = lp.A.row_count();
  set_initial_x();

  for (int round = 0; round < max_rounds; round++) {
    // Set up base + inverse
    // std::sort(basic_indices.begin(), basic_indices.end());
    ColMatrix m(row_count, 0);
    for (auto i : basic_indices)
      m.add_column(lp.A.column(i));
    Base base(m);
    const bool is_regular = base.invert();
    assert(is_regular);

    if (round > 9999) {
      auto prod = base.get_inverse() * m;
      if (ColMatrix::identity(row_count) != prod) {
        Debug(1) << "Inverse * Base failed, result:\n" << prod << "\n";
      }
    }
    // Calc beta
    DVector beta(row_count);
    beta = lp.b;
    for (auto i : non_basic_indices)
      beta -= x[i] * lp.A.column(i);
    base.ftran(beta);

    for (size_t i = 0; i < basic_indices.size(); i++)
      x[basic_indices[i]] = beta[i];

    if (phase == Phase::Two)
      assert(lp.is_feasible(x));

    // Set c for phase I
    if (phase == Simplex::Phase::One) {
      for (auto i : non_basic_indices)
        c[i] = 0.0;
      for (size_t i = 0; i < basic_indices.size(); i++) {
        const auto column_header = lp.column_header(basic_indices[i]);
        if (is_finite(column_header.lower) &&
            is_lower_norm(beta[i], column_header.lower)) {
          c[basic_indices[i]] = -1.0;
        } else if (is_finite(column_header.upper) &&
                   is_greater_norm(beta[i], column_header.upper)) {
          c[basic_indices[i]] = 1.0;
        } else {
          c[basic_indices[i]] = 0.0;
        }
      }
    }

    z = c * x;

    // Price
    DVector pi(row_count);
    DVector d(col_count);
    for (size_t i = 0; i < row_count; i++)
      pi[i] = c[basic_indices[i]]; // ==c_B
    base.btran(pi);
    for (size_t i = 0; i < col_count; i++)
      d[i] = c[i] - pi * lp.A.column(i);

    Pricer pricer;
    auto pr = pricer.price(x, lp, d, non_basic_indices);

    IterationDecision iteration_decision = IterationDecision::Unfinished;

    if (pr.is_optimal) {
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
    // Transform column vector of improving candidate
    SVector alpha = lp.A.column(pr.candidate_index);
    base.ftran(alpha);

    // Ratio test
    const auto rt =
        RatioTester().ratio_test(lp, alpha, beta, pr.candidate_index,
                                 basic_indices, d[pr.candidate_index]);
    if (iteration_decision == IterationDecision::Unfinished)
      iteration_decision = rt.result;

    print_iteration_results(iteration_decision, round);

    switch (iteration_decision) {
    case IterationDecision::BaseChange: {
      x[basic_indices[rt.leaving_index]] = rt.leaving_bound;
      const auto it = std::find(non_basic_indices.begin(),
                                non_basic_indices.end(), pr.candidate_index);

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
      x[pr.candidate_index] = rt.step_length;
      z = -inf;
      return;
    case IterationDecision::BoundFlip:
      x[pr.candidate_index] += rt.step_length;
      break;
    case IterationDecision::OptimalSolution:
      result = Result::OptimalSolution;
      return;
    case IterationDecision::SwitchToPhaseTwo:
      phase = Simplex::Phase::Two;
      c = lp.c;
      continue;
    case IterationDecision::Infeasible:
      result = Result::Infeasible;
      return;
    default:
      assert(false);
    }
  }
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
