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

  DVector beta(row_count);
  DVector c_beta(row_count);
  DVector d(col_count);
  DVector pi(row_count);

  for (int round = 0; round < 999; round++) {
    std::cout << "\n\nIteration " << round;
    std::cout << "\n-----------------------------------------\n\n";

    // Set up base + inverse
    ColMatrix m(row_count, 0);
    for (const auto &i : basic_indices)
      m.add_column(lp.A.column(i));
    Base base(m);
    base.invert();

    beta = lp.b;

    base.ftran(beta);
    std::cout << beta << std::endl;

    for (size_t i = 0; i < row_count; i++) {
      c_beta[i] = lp.c[basic_indices[i]];
    }

    z = c_beta * beta;

    pi = c_beta;
    base.btran(pi);

    double min_val = 0.0f;
    size_t min_posi = 0;
    for (size_t i = 0; i < non_basic_indices.size(); i++) {
      d[i] =
          lp.c[non_basic_indices[i]] - pi * lp.A.column(non_basic_indices[i]);
      if (d[i] < min_val) {
        min_val = d[i];
        min_posi = i;
      }
    }
    std::cout << "B:\n" << base.get_base() << std::endl;
    std::cout << "z = " << z << std::endl;
    std::cout << "pi = " << pi << std::endl;
    std::cout << "d = " << d << "\n\n";
    std::cout << "selected new basic index: " << min_posi << "\n\n";

    if (is_zero(min_val)) {
      for (size_t i = 0; i < basic_indices.size(); i++)
        x[basic_indices[i]] = beta[i];
      result = Result::OptimalSolution;
      return;
    } else {
      SVector alpha = lp.A.column(min_posi);
      std::cout << "alpha = \n" << alpha << "\n\n";
      base.ftran(alpha);
      std::cout << "updated alpha = \n" << alpha << "\n\n";

      double min_theta = inf;
      size_t min_theta_posi = 0;
      for (const auto &n : alpha) {
        double t = beta[n.index] / n.value;
        if (t < min_theta) {
          min_theta = t;
          min_theta_posi = n.index;
        }
        std::cout << t << " ";
      }
      std::cout << "\nMin theta = " << min_theta << " at position "
                << min_theta_posi << "\n\n";
      if (is_zero(min_theta)) {
        // Bound flip or unbounded
      } else {
        size_t temp = non_basic_indices[min_posi];
        non_basic_indices[min_posi] = basic_indices[min_theta_posi];
        basic_indices[min_theta_posi] = temp;
        std::cout << "Basic:\n";
        for (auto v : basic_indices)
          std::cout << v << " ";
        std::cout << "\n\nNon-Basic:\n";
        for (auto v : non_basic_indices)
          std::cout << v << " ";
      }
    }
  }
}
