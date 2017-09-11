#include "simplex.h"
#include "base.h"
#include <iostream>

Simplex::Simplex(const LP &_lp)
    : print_iterations(false), lp(_lp), x(lp.A.col_count()), z(inf),
      result(Result::Unsolved),
      structural_count(lp.A.col_count() - lp.A.row_count()),
      row_count(lp.A.row_count()), col_count(lp.A.col_count()) {}

const DVector &Simplex::get_x() const { return x; }

const double &Simplex::get_z() const { return z; }

const Simplex::Result &Simplex::get_result() const { return result; }

void Simplex::solve() {
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
    base.ftran(beta);

    for (size_t i = 0; i < row_count; i++)
      c_beta[i] = lp.c[basic_indices[i]];

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
    }

    // Price
    pi = c_beta;
    base.btran(pi);
    for (size_t i = 0; i < non_basic_indices.size(); i++)
      d[i] =
          lp.c[non_basic_indices[i]] - pi * lp.A.column(non_basic_indices[i]);

    if (print_iterations) {
      std::cout << "pi = " << pi << std::endl;
      std::cout << "d = " << d << "\n\n";
    }

    auto pr = price(d, non_basic_indices);

    if (pr.is_optimal) {
      for (size_t i = 0; i < basic_indices.size(); i++)
        x[basic_indices[i]] = beta[i];
      result = Result::OptimalSolution;
      z = lp.c * x;
      return;
    } else {
      // Transform column vector of improving candidate
      alpha = lp.A.column(pr.candidate_index);
      base.ftran(alpha);
      if (print_iterations) {
        std::cout << "Alpha = \n" << alpha << "\n\n";
      }

      // Ratio test
      auto rt = ratio_test(alpha, beta, non_basic_indices[pr.candidate_index],
                           basic_indices, d[pr.candidate_index]);
      if (print_iterations) {
        std::cout << "selected new basic index: " << pr.candidate_index
                  << "\n\n";
      }

      switch (rt.result) {
      case IterationResult::BaseChange:
        std::swap<size_t>(non_basic_indices[pr.candidate_index],
                          basic_indices[rt.leaving_index]);
        break;
      case IterationResult::Unbounded:
        result = Result::Unbounded;
        z = -inf;
        return;
      case IterationResult::BoundFlip:
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
    double sign = 1.0f;
    if (is_eq(x[j], lp.column_header(j).upper))
      sign = -1.0f; // move from upper bound down
    if (sign * d[i] < min_val) {
      min_val = sign * d[i];
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
  double a = inf;
  const double direction = is_ge(candidate_cost, 0.0f) ? -1.0f : 1.0f;

  for (const auto &n : alpha) {
    a = n.value * direction;
    if (is_le(a, 0.0f)) { // moving towards i's lower bound
      bound = lp.column_header(basic_indices[n.index]).upper;
    } else {
      bound = lp.column_header(basic_indices[n.index]).lower;
    }
    double t = (beta[n.index] - bound) / a;
    if (t < min_theta) {
      min_theta = t;
      min_theta_posi = n.index;
    }
  }
  if (print_iterations) {
    std::cout << "\nMin theta = " << min_theta << " at position "
              << min_theta_posi << "\n\n";
  }
  RatioTestResult rt;
  if (is_infinite(min_theta)) {
    if (is_infinite(lp.column_header(candidate_index).upper)) {
      rt.result = IterationResult::Unbounded;
    }
  } else {
    rt.leaving_index = min_theta_posi;
    rt.result = IterationResult::BaseChange;
  }
  return rt;
}
