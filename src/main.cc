#include <iostream>
#include <random>

#include "base.h"
#include "colmatrix.h"
#include "lp.h"
#include "svector.h"
#include "test.h"

int main() {
  TestRunner::run();

  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_row(RowType::LE, -inf, 10);
  lp.add_row(RowType::LE, -inf, 15);

  lp.add_value(0, 0, 3);
  lp.add_value(0, 1, 2);
  lp.add_value(0, 2, 1);
  lp.add_value(1, 0, 2);
  lp.add_value(1, 1, 5);
  lp.add_value(1, 2, 3);

  lp.add_logicals();

  lp.add_obj_value(0, -2);
  lp.add_obj_value(1, -3);
  lp.add_obj_value(2, -4);

  lp.set_b();

  std::cout << "A:\n" << lp.A;
  std::cout << "b:\n";
  for (size_t i = 0; i < lp.b.dimension(); i++)
    std::cout << lp.b[i] << " ";
  std::cout << std::endl;

  std::cout << "c:\n";
  for (size_t i = 0; i < lp.c.dimension(); i++)
    std::cout << lp.c[i] << " ";
  std::cout << std::endl;

  // Index sets
  std::vector<size_t> basic_indices;
  basic_indices.push_back(3);
  basic_indices.push_back(4);
  std::vector<size_t> non_basic_indices;
  non_basic_indices.push_back(0);
  non_basic_indices.push_back(1);
  non_basic_indices.push_back(2);

  DVector x(lp.c.dimension());
  std::cout << "x = \n" << x << "\n\n";

  for (int round = 0; round < 2; round++) {
    ColMatrix m(2, 0);
    for (const auto &i : basic_indices)
      m.add_column(lp.A.column(i));

    Base base(m);
    std::cout << "B:\n" << base.get_base() << std::endl;

    base.invert();

    DVector beta(2);
    beta = lp.b;

    base.ftran(beta);
    std::cout << beta << std::endl;

    const size_t rows = basic_indices.size();
    DVector c(rows);
    for (size_t i = 0; i < rows; i++) {
      c[i] = lp.c[basic_indices[i]];
    }

    double z = c * beta;
    std::cout << "z = " << z << std::endl;

    DVector pi = c;
    base.btran(pi);
    std::cout << "pi = " << pi << std::endl;

    DVector d(non_basic_indices.size());
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
    std::cout << "d = " << d << "\n\n";
    std::cout << "selected new basic index: " << min_posi << "\n\n";

    if (is_zero(min_val)) {
      std::cout << "Optimal solution found!\n";
      std::cout << "Value = " << z << "\n\n";
      for (size_t i = 0; i < basic_indices.size(); i++)
        x[basic_indices[i]] = beta[i];
      std::cout << "x = \n" << x << "\n\n";
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
  return 0;
}
