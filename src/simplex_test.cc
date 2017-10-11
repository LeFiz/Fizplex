#include "simplex.h"
#include "test.h"

// TODO LP from string: LP lp("min x - 2*y; x+3*y = 1; 0<=x<=inf, -inf<=y<=inf")

Test(Simplex, solve, "Ax <= b, x >= 0, unbounded") {
  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf, -1);
  lp.add_row(RowType::LE, -inf, 14, {-2});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::Unbounded);
  EXPECT(is_infinite(splx.get_z()));
}

Test(Simplex, solve, "Ax <= b, x <= 0, bounded") {
  LP lp;
  lp.add_column(ColType::UpperBound, -inf, 0.0, 1);
  lp.add_row(RowType::LE, -inf, 14, {-2});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -7.0));
}

Test(Simplex, solve, "Ax <= b, l <= x <= u, bounded, boundflip") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5, -1);
  lp.add_row(RowType::LE, -inf, 14, {2});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -5.0));
}

Test(Simplex, solve, "Ax == b, l <= x <= u, bounded") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5, -1);
  lp.add_column(ColType::Bounded, 0, 5, 0);
  lp.add_row(RowType::Equality, 0, 0, {1, -1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -5));
  EXPECT(splx.get_x() == DVector({5.0, 5.0, 0.0}));
}

Test(Simplex, solve, "Ax >= b, x <= 0, bounded") {
  LP lp;
  lp.add_column(ColType::UpperBound, -inf, 0, 1);
  lp.add_row(RowType::GE, -5, inf, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -5.0));
}

Test(Simplex, solve, "l <= Ax <= u, l <= x <= u, bounded") {
  LP lp;
  lp.add_column(ColType::Bounded, -4, 6, -5);
  lp.add_row(RowType::Range, -5, 5, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -25.0));
  EXPECT(splx.get_x() == DVector({5.0, 0.0}));
}

Test(Simplex, solve, "-inf <= Ax <= inf, x free, unbounded") {
  LP lp;
  lp.add_column(ColType::Free, -inf, inf, 1);
  lp.add_row(RowType::NonBinding, -inf, inf, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::Unbounded);
  EXPECT(is_infinite(splx.get_z()));
  EXPECT(is_le(splx.get_z(), 0));
  EXPECT(is_infinite(splx.get_x()[0]));
}

Test(Simplex, solve, "Ax <= b, x == 0, bounded") {
  LP lp;
  lp.add_column(ColType::Fixed, 0, 0, -1);
  lp.add_row(RowType::Range, -5, 5, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), 0));
  EXPECT(is_eq(splx.get_x()[0], 0));
}

Test(Simplex, solve, "Ax >= b, 0 <= x <= 1, bounded") {
  LP lp;
  lp.add_column(ColType::LowerBound, 1.0, inf, 2);
  lp.add_column(ColType::LowerBound, 1.0, inf, 1);
  lp.add_row(RowType::Equality, 3, 3, {1, 1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), 4.0));
}

Test(Simplex, solve, "2-phase, 4 var, 4 constr") {
  LP lp;
  lp.add_column(ColType::Free, -inf, inf, 3);
  lp.add_column(ColType::Free, -inf, inf, -2);
  lp.add_column(ColType::LowerBound, 0.0, inf, 1);
  lp.add_column(ColType::LowerBound, 0.0, inf, -4);
  lp.add_row(RowType::GE, 4, inf, {1, 1, -4, 2});
  lp.add_row(RowType::LE, -inf, 6, {-3, 1, -2, 0});
  lp.add_row(RowType::Equality, -1, -1, {0, 1, 0, -1});
  lp.add_row(RowType::Equality, 0, 0, {1, 1, -1, 0});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -32));
}

Test(Simplex, solve, "infeasible") {
  LP lp;
  lp.add_column(ColType::LowerBound, 1.0, inf, 2);
  lp.add_row(RowType::LE, -inf, 0, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::Infeasible);
}

Test(Simplex, solve, "infeasible") {
  LP lp;
  lp.add_column(ColType::Fixed, -3.0, -3.0, 2);
  lp.add_row(RowType::Equality, 0, 0, {2.5});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::Infeasible);
}

Test(Simplex, solve, "Ax <= b, x >= 0, optimal Solution exists") {
  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf, -1);
  lp.add_column(ColType::LowerBound, 0, inf, -2);
  lp.add_column(ColType::LowerBound, 0, inf, 3);
  lp.add_row(RowType::LE, -inf, 14, {2, 1, 1});
  lp.add_row(RowType::LE, -inf, 28, {4, 2, 3});
  lp.add_row(RowType::LE, -inf, 30, {2, 5, 5});

  Simplex splx(lp);
  splx.solve();

  EXPECT(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT(is_eq(splx.get_z(), -13));
}
