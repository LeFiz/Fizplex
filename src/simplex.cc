#include "simplex.h"
#include "base.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

Simplex::Simplex(LP &_lp) : lp(_lp) {}

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
  lp.add_logicals();
  const size_t col_count = lp.A.col_count();
  const size_t row_count = lp.A.row_count();
  const size_t structural_count = col_count - row_count;
  x = DVector(col_count);

  // Set up index sets
  std::vector<size_t> basic_indices;
  std::vector<size_t> non_basic_indices;
  for (size_t i = 0; i < structural_count; i++)
    non_basic_indices.push_back(i);
  for (size_t i = structural_count; i < col_count; i++)
    basic_indices.push_back(i);

  // Set up vectors
  DVector beta(row_count);
  DVector pi(row_count);
  DVector d(col_count);
  SVector alpha;
  DVector c(col_count);

  set_initial_x();

  for (int round = 0; round < max_rounds; round++) {
    // Set up base + inverse
    ColMatrix m(row_count, 0);
    for (auto i : basic_indices)
      m.add_column(lp.A.column(i));
    Base base(m);
    base.invert();

    // Calc beta
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
            is_lower(beta[i], column_header.lower)) {
          c[basic_indices[i]] = -1.0;
        } else if (is_finite(column_header.upper) &&
                   is_greater(beta[i], column_header.upper)) {
          c[basic_indices[i]] = 1.0;
        } else {
          c[basic_indices[i]] = 0.0;
        }
      }
    }

    z = c * x;

    // Price
    for (size_t i = 0; i < row_count; i++)
      pi[i] = c[basic_indices[i]]; // ==c_B
    base.btran(pi);
    for (size_t i = 0; i < col_count; i++)
      d[i] = c[i] - pi * lp.A.column(i);

    auto pr = price(d, non_basic_indices);

    if (pr.is_optimal) {
      if (phase == Simplex::Phase::Two) {
        result = Result::OptimalSolution;
        return;
      } else { // Phase I
        if (lp.is_feasible(x)) {
          phase = Simplex::Phase::Two;
          c = lp.c;
          continue;
        } else {
          result = Result::Infeasible;
          return;
        }
      }
    } else {
      // Transform column vector of improving candidate
      alpha = lp.A.column(pr.candidate_index);
      base.ftran(alpha);

      // Ratio test
      const auto rt = ratio_test(alpha, beta, pr.candidate_index, basic_indices,
                                 d[pr.candidate_index]);

      switch (rt.result) {
      case IterationResult::BaseChange: {
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
      case IterationResult::Unbounded:
        assert(phase != Simplex::Phase::One);

        result = Result::Unbounded;
        x[basic_indices[rt.leaving_index]] = rt.leaving_bound;
        x[pr.candidate_index] = rt.step_length;
        z = -inf;
        return;
      case IterationResult::BoundFlip:
        x[pr.candidate_index] += rt.step_length;
        break;
      default:
        assert(false);
      }
    }
  }
}

Simplex::PricingResult
Simplex::price(DVector &d, std::vector<size_t> &non_basic_indices) const {
  double min_val = 0.0;
  size_t min_posi = 0;
  for (auto j : non_basic_indices) {
    if (lp.column_header(j).type == ColType::Fixed)
      continue; // Fixed vars should not enter the basis

    double sign = 1.0;
    if ((is_eq(x[j], lp.column_header(j).upper)) ||
        (lp.column_header(j).type == ColType::Free && is_ge(d[j], 0.0)))
      sign = -1.0;

    if (sign * d[j] < min_val) {
      min_val = sign * d[j];
      min_posi = j;
    }
  }
  PricingResult pr;
  pr.is_optimal = is_zero(min_val);
  pr.candidate_index = min_posi;
  return pr;
}

Simplex::RatioTestResult Simplex::ratio_test(SVector &alpha, DVector &beta,
                                             size_t candidate_index,
                                             std::vector<size_t> &basic_indices,
                                             double candidate_cost) const {
  double min_theta = inf;
  size_t min_theta_posi = 0;
  double bound = inf;
  double leaving_bound = inf;
  double a = inf;
  const double direction = is_ge(candidate_cost, 0.0) ? -1.0 : 1.0;

  for (const auto &n : alpha) {
    if (is_zero(n.value))
      continue; // alpha_i = 0 can not limit displacement

    const auto &column_header = lp.column_header(basic_indices[n.index]);
    if (column_header.type == ColType::Fixed) {
      min_theta = 0; // Fixed variable always means step_length = 0
      min_theta_posi = n.index;
      leaving_bound = column_header.lower;
      break;
    }

    a = n.value * direction;
    if (is_le(a, 0.0)) {
      bound = column_header.upper; // = inf for ColType = Lower
    } else {
      bound = column_header.lower; // = -inf for ColType = upper
    }
    double t = (beta[n.index] - bound) / a;
    if (t < min_theta) {
      min_theta = t;
      min_theta_posi = n.index;
      leaving_bound = bound;
      assert(is_ge(min_theta, 0.0));
    }
  }

  const double max_steplength = lp.column_header(candidate_index).upper -
                                lp.column_header(candidate_index).lower;
  min_theta = std::min<double>(min_theta, max_steplength);

  RatioTestResult rt;
  rt.step_length = direction * min_theta; // make signed again
  rt.leaving_bound = leaving_bound;
  if (is_infinite(min_theta)) {
    rt.result = IterationResult::Unbounded;
    rt.leaving_index = min_theta_posi;
  } else if (is_eq(min_theta, max_steplength)) {
    rt.result = IterationResult::BoundFlip;
  } else {
    rt.result = IterationResult::BaseChange;
    rt.leaving_index = min_theta_posi;
  }
  return rt;
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
