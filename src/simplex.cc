#include "simplex.h"
#include "base.h"
#include <iostream>

Simplex::Simplex(const LP &_lp)
    : lp(_lp), x(lp.A.col_count()), z(inf), result(Result::Unsolved),
      structural_count(lp.A.col_count() - lp.A.row_count()),
      row_count(lp.A.row_count()), col_count(lp.A.col_count()) {}

const DVector &Simplex::get_x() const { return x; }

const double &Simplex::get_z() const { return z; }

const Simplex::Result &Simplex::get_result() const { return result; }

void Simplex::solve() {
  std::cout << "A:\n" << lp.A;
  std::cout << "b = " << lp.b << std::endl;
  std::cout << "c = " << lp.c << std::endl;

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
  SVector alpha;

  for (int round = 0; round < 999; round++) {
    std::cout << "\n\nIteration " << round;
    std::cout << "\n-----------------------------------------\n\n";

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
    std::cout << "B:\n" << base.get_base() << std::endl;
    std::cout << "Beta: " << beta << std::endl;
    std::cout << "z = " << z << std::endl;
    std::cout << "Basic:\n";
    for (auto v : basic_indices)
      std::cout << v << " ";
    std::cout << "\n\nNon-Basic:\n";
    for (auto v : non_basic_indices)
      std::cout << v << " ";

    // Price
    pi = c_beta;
    base.btran(pi);
    auto pr = price(pi, non_basic_indices);

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
      std::cout << "Alpha = \n" << alpha << "\n\n";

      // Ratio test
      auto rt = ratio_test(alpha, beta);
      std::cout << "selected new basic index: " << pr.candidate_index << "\n\n";

      if (rt.result == IterationResult::BaseChange) {
        std::swap<size_t>(non_basic_indices[pr.candidate_index],
                          basic_indices[rt.leaving_index]);
      }
    }
  }
}

Simplex::PricingResult
Simplex::price(DVector &pi, std::vector<size_t> &non_basic_indices) const {
  DVector d(col_count);
  double min_val = 0.0f;
  size_t min_posi = 0;
  for (size_t i = 0; i < non_basic_indices.size(); i++) {
    d[i] = lp.c[non_basic_indices[i]] - pi * lp.A.column(non_basic_indices[i]);
    if (d[i] < min_val) {
      min_val = d[i];
      min_posi = i;
    }
  }
  PricingResult pr;
  pr.is_optimal = is_zero(min_val);
  pr.candidate_index = min_posi;
  std::cout << "pi = " << pi << std::endl;
  std::cout << "d = " << d << "\n\n";
  return pr;
}

Simplex::RatioTestResult Simplex::ratio_test(SVector &alpha,
                                             DVector &beta) const {
  double min_theta = inf;
  size_t min_theta_posi = 0;
  for (const auto &n : alpha) {
    double t = beta[n.index] / n.value;
    if (t < min_theta) {
      min_theta = t;
      min_theta_posi = n.index;
    }
  }
  std::cout << "\nMin theta = " << min_theta << " at position "
            << min_theta_posi << "\n\n";
  RatioTestResult rt;
  if (is_zero(min_theta)) {
    // bound flip or unbounded
  } else {
    rt.leaving_index = min_theta_posi;
    rt.result = IterationResult::BaseChange;
  }
  return rt;
}
