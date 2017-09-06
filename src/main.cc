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
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_row(RowType::LE, -inf, 14);
  lp.add_row(RowType::LE, -inf, 28);
  lp.add_row(RowType::LE, -inf, 30);

  lp.add_value(0, 0, 2);
  lp.add_value(0, 1, 1);
  lp.add_value(0, 2, 1);
  lp.add_value(1, 0, 4);
  lp.add_value(1, 1, 2);
  lp.add_value(1, 2, 3);
  lp.add_value(2, 0, 2);
  lp.add_value(2, 1, 5);
  lp.add_value(2, 2, 5);

  lp.add_logicals();

  lp.add_obj_value(0, -1);
  lp.add_obj_value(1, -2);
  lp.add_obj_value(2, 3);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();
  return 0;
}
