#include "simplex.h"
#include "test.h"

Test(Simplex, solve, "Ax <= b, x >= 0, optimal Solution exists") {
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

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -13));
}

Test(Simplex, solve, "Ax <= b, x >= 0, unbounded") {
  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_row(RowType::LE, -inf, 14);

  lp.add_value(0, 0, -2);

  lp.add_logicals();

  lp.add_obj_value(0, -1);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::Unbounded);
  EXPECT(is_infinite(splx.get_z()));
}

Test(Simplex, solve, "Ax <= b, x <= 0, bounded") {
  LP lp;
  lp.add_column(ColType::UpperBound, -inf, 0.0f);
  lp.add_row(RowType::LE, -inf, 14);

  lp.add_value(0, 0, -2);

  lp.add_logicals();

  lp.add_obj_value(0, 1);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -7.0f));
}

Test(Simplex, solve, "Ax <= b, l <= x <= u, bounded") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::LE, -inf, 14);

  lp.add_value(0, 0, 2);

  lp.add_logicals();

  lp.add_obj_value(0, -1);

  lp.set_b();

  Simplex splx(lp);
  splx.print_iterations = true;
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  std::cout << "\nz=" << splx.get_z() << "\n";
  EXPECT(is_eq(splx.get_z(), -5.0f));
}
