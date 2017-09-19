#include "simplex.h"
#include "test.h"

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

Test(Simplex, solve, "Ax <= b, l <= x <= u, bounded, boundflip") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::LE, -inf, 14);

  lp.add_value(0, 0, 2);

  lp.add_logicals();

  lp.add_obj_value(0, -1);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -5.0f));
}

Test(Simplex, solve, "Ax == b, l <= x <= u, bounded") {
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
  splx.solve();
  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -10));
  EXPECT(splx.get_x() == DVector({5.0f, -5.0f, 0.0f}));
}

Test(Simplex, solve, "Ax >= b, x <= 0, bounded") {
  LP lp;
  lp.add_column(ColType::UpperBound, -inf, 0);
  lp.add_row(RowType::GE, -5, inf);

  lp.add_value(0, 0, 1);

  lp.add_logicals();

  lp.add_obj_value(0, 1);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -5.0f));
}

Test(Simplex, solve, "l <= Ax <= u, l <= x <= u, bounded") {
  LP lp;
  lp.add_column(ColType::Bounded, -4, 6);
  lp.add_row(RowType::Range, -5, 5);

  lp.add_value(0, 0, 1);

  lp.add_logicals();

  lp.add_obj_value(0, -5);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -25.0f));
  EXPECT(splx.get_x() == DVector({5.0f, 0.0f}));
}

Test(Simplex, solve, "-inf <= Ax <= inf, x free, unbounded") {
  LP lp;
  lp.add_column(ColType::Free, -inf, inf);
  lp.add_row(RowType::NonBinding, -inf, inf);

  lp.add_value(0, 0, 1);

  lp.add_logicals();

  lp.add_obj_value(0, 1);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::Unbounded);
  EXPECT(is_infinite(splx.get_z()));
  EXPECT(is_le(splx.get_z(), 0));
  EXPECT(is_infinite(splx.get_x()[0]));
}

Test(Simplex, solve, "Ax <= b, x == 0, bounded") {
  LP lp;
  lp.add_column(ColType::Fixed, 0, 0);
  lp.add_row(RowType::Range, -5, 5);

  lp.add_value(0, 0, 1);

  lp.add_logicals();

  lp.add_obj_value(0, -1);

  lp.set_b();

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), 0));
  EXPECT(is_eq(splx.get_x()[0], 0));
}

Test(Simplex, solve, "Ax >= b, 0 <= x <= 1, bounded") {
  LP lp;
  lp.add_column(ColType::LowerBound, 1.0f, inf);
  lp.add_column(ColType::LowerBound, 1.0f, inf);
  lp.add_row(RowType::Equality, 3, 3);

  lp.add_value(0, 0, 1);
  lp.add_value(0, 1, 1);

  lp.add_logicals();

  lp.add_obj_value(0, 1);
  lp.add_obj_value(1, 2);

  lp.set_b();

  Simplex splx(lp);
  splx.print_iterations = true;
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), 4.0f));
}

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
