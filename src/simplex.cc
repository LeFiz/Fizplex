#include "simplex.h"
#include "base.h"
#include <iostream>

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
      x[i] = 0.0f;
    }
  }
}

void Simplex::solve() {
  lp.add_logicals();
  const size_t col_count = lp.A.col_count();
  const size_t row_count = lp.A.row_count();
  const size_t structural_count = col_count - row_count;
  x = DVector(col_count);

  if (print_iterations) {
    std::cout << "A:\n" << lp.A;
    std::cout << "b = " << lp.b << std::endl;
    std::cout << "c = " << lp.c << std::endl;
  }

  // Set up index sets
  std::vector<size_t> basic_indices;
  std::vector<size_t> non_basic_indices;
  for (size_t i = 0; i < structural_count; i++)
    non_basic_indices.push_back(i);
  for (size_t i = structural_count; i < col_count; i++)
    basic_indices.push_back(i);

  // Set up vectors
  DVector beta(row_count);
  DVector c_beta(row_count);
  DVector pi(col_count);
  DVector d(col_count);
  SVector alpha;
  DVector c(col_count);

  set_initial_x();
  if (lp.is_feasible(x)) {
    phase = Simplex::Phase::Two;
    c = lp.c;
  }

  for (int round = 0; round < 10; round++) {
    if (print_iterations) {
      std::cout << "\n\nIteration " << round;
      std::cout << "\n-----------------------------------------\n\n";
    }

    // Set up base + inverse
    ColMatrix m(row_count, 0);
    for (const auto &i : basic_indices)
      m.add_column(lp.A.column(i));
    Base base(m);
    base.invert();

    // Calc beta
    beta = lp.b;
    for (auto &i : non_basic_indices)
      beta -= x[i] * lp.A.column(i);
    base.ftran(beta);

    // Set c for phase I
    if (phase == Simplex::Phase::One) {
      for (auto &i : non_basic_indices)
        c[i] = 0.0f;
      for (size_t i = 0; i < basic_indices.size(); i++) {
        auto column_header = lp.column_header(basic_indices[i]);
        if (is_finite(column_header.lower) &&
            is_lower(beta[i], column_header.lower)) {
          c[basic_indices[i]] = -1.0f;
        } else if (is_finite(column_header.upper) &&
                   is_greater(beta[i], column_header.upper)) {
          c[basic_indices[i]] = 1.0f;
        } else {
          c[basic_indices[i]] = 0.0f;
        }
      }
    }

    // Report iteration status
    if (print_iterations) {
      std::cout << "B:\n" << base.get_base() << std::endl;
      std::cout << "Beta: " << beta << std::endl;
      std::cout << "Basic:\n";
      for (auto v : basic_indices)
        std::cout << v << " ";
      std::cout << "\n\nNon-Basic:\n";
      for (auto v : non_basic_indices)
        std::cout << v << " ";
      std::cout << "\n\n";
    }

    // Price
    for (size_t i = 0; i < row_count; i++)
      c_beta[i] = c[basic_indices[i]];
    pi = c_beta;
    base.btran(pi);
    for (size_t i = 0; i < non_basic_indices.size(); i++)
      d[non_basic_indices[i]] =
          c[non_basic_indices[i]] - pi * lp.A.column(non_basic_indices[i]);

    if (print_iterations) {
      std::cout << "c = " << c << std::endl;
      std::cout << "pi = " << pi << std::endl;
      std::cout << "d = " << d << "\n\n";
    }

    for (size_t i = 0; i < basic_indices.size(); i++)
      x[basic_indices[i]] = beta[i];
    z = lp.c * x;

    auto pr = price(d, non_basic_indices);

    if (print_iterations) {
      std::cout << "x = " << x << std::endl;
      std::cout << "z = " << z << "\n";
      std::cout << "optimal? = " << pr.is_optimal << "\n\n";
    }

    if (pr.is_optimal) {
      if (phase == Simplex::Phase::Two) {
        result = Result::OptimalSolution;
        return;
      } else { // Phase I
        if (lp.is_feasible(x)) {
          if (print_iterations)
            std::cout << "is feasible, x = " << x << "\n";
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
      alpha = lp.A.column(non_basic_indices[pr.candidate_index]);
      base.ftran(alpha);
      if (print_iterations) {
        std::cout << "Alpha = \n" << alpha << "\n\n";
      }

      // Ratio test
      auto rt =
          ratio_test(alpha, beta, non_basic_indices[pr.candidate_index],
                     basic_indices, d[non_basic_indices[pr.candidate_index]]);
      if (print_iterations) {
        std::cout << "selected new basic index: "
                  << non_basic_indices[pr.candidate_index] << "\n\n";
      }

      switch (rt.result) {
      case IterationResult::BaseChange:
        x[basic_indices[rt.leaving_index]] = rt.leaving_bound;
        std::swap<size_t>(non_basic_indices[pr.candidate_index],
                          basic_indices[rt.leaving_index]);
        break;
      case IterationResult::Unbounded:
        assert(phase != Simplex::Phase::One);

        result = Result::Unbounded;
        x[basic_indices[rt.leaving_index]] = rt.leaving_bound;
        x[non_basic_indices[pr.candidate_index]] = rt.step_length;
        z = -inf;
        return;
      case IterationResult::BoundFlip:
        x[non_basic_indices[pr.candidate_index]] += rt.step_length;
        break;
      default:
        assert(false);
      }
    }
  }
}

Simplex::PricingResult
Simplex::price(DVector &d, std::vector<size_t> &non_basic_indices) const {
  double min_val = 0.0f;
  size_t min_posi = 0;
  for (size_t i = 0; i < non_basic_indices.size(); i++) {
    size_t j = non_basic_indices[i];
    if (lp.column_header(j).type ==
        ColType::Fixed) // Fixed vars should not enter the basis
      continue;
    double sign = 1.0f;
    if ((is_eq(x[j], lp.column_header(j).upper)) ||
        (lp.column_header(j).type == ColType::Free && is_ge(d[j], 0.0f)))
      sign = -1.0f;
    if (sign * d[j] < min_val) {
      min_val = sign * d[j];
      min_posi = i;
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
  const double direction = is_ge(candidate_cost, 0.0f) ? -1.0f : 1.0f;

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
    if (is_le(a, 0.0f)) {
      bound = column_header.upper; // = inf for ColType = Lower
    } else {
      bound = column_header.lower; // = -inf for ColType = upper
    }
    double t = (beta[n.index] - bound) / a;
    if (t < min_theta) {
      min_theta = t;
      min_theta_posi = n.index;
      leaving_bound = bound;
      if (print_iterations)
        std::cout << "\nmin_theta updated: " << min_theta << " at "
                  << basic_indices[min_theta_posi] << "\n";
      assert(is_ge(min_theta, 0.0f));
    }
  }

  const double max_steplength = lp.column_header(candidate_index).upper -
                                lp.column_header(candidate_index).lower;
  min_theta = std::min<double>(min_theta, max_steplength);

  if (print_iterations) {
    std::cout << "\nMin theta = " << min_theta << " at position "
              << min_theta_posi << "\n\n";
  }

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
