#include <array>
#include <iostream>
#include <random>

#include "base.h"
#include "colmatrix.h"
#include "lp.h"
#include "simplex.h"
#include "svector.h"
#include "test.h"

int main() {
  TestRunner::run();

  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_column(ColType::Bounded, -5, 0);
  lp.add_row(RowType::Equality, 0, 0);

  lp.add_value(0, 0, 1);
  lp.add_value(0, 1, 1);

  lp.add_logicals();

  lp.add_obj_value(0, -1);
  lp.add_obj_value(1, 1);

  lp.set_b();

  Simplex splx(lp);
  splx.print_iterations = true;
  splx.solve();

  std::cout << "\nOpt solution? "
            << (splx.get_result() == Simplex::Result::OptimalSolution) << "\n";
  std::cout << "z = " << splx.get_z() << "\nx = " << splx.get_x() << "\n";
  return 0;
}
