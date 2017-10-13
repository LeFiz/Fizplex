#include "simplex.h"
#include "gtest/gtest.h"

// TODO LP from string: LP lp("min x - 2*y; x+3*y = 1; 0<=x<=inf, -inf<=y<=inf")

TEST(SimplexTest, LowerRowLowerVarUnbounded) {
  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf, -1);
  lp.add_row(RowType::LE, -inf, 14, {-2});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::Unbounded);
  EXPECT_TRUE(is_infinite(splx.get_z()));
}

TEST(SimplexTest, LowerRowUpperVarOptimalSolution) {
  LP lp;
  lp.add_column(ColType::UpperBound, -inf, 0.0, 1);
  lp.add_row(RowType::LE, -inf, 14, {-2});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -7.0));
}

TEST(SimplexTest, LowerRowBoundedVarOptimalSolutionWithBoundFlip) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5, -1);
  lp.add_row(RowType::LE, -inf, 14, {2});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -5.0));
}

TEST(SimplexTest, EqualityRowBoundedVarsOptimalSolution) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5, -1);
  lp.add_column(ColType::Bounded, 0, 5, 0);
  lp.add_row(RowType::Equality, 0, 0, {1, -1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -5));
  EXPECT_TRUE(splx.get_x() == DVector({5.0, 5.0, 0.0}));
}

TEST(SimplexTest, GreaterRowUpperVarOptimalSolution) {
  LP lp;
  lp.add_column(ColType::UpperBound, -inf, 0, 1);
  lp.add_row(RowType::GE, -5, inf, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -5.0));
}

TEST(SimplexTest, RangeRowBoundedVarOptimalSolution) {
  LP lp;
  lp.add_column(ColType::Bounded, -4, 6, -5);
  lp.add_row(RowType::Range, -5, 5, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -25.0));
  EXPECT_TRUE(splx.get_x() == DVector({5.0, 0.0}));
}

TEST(SimplexTest, NonBindingRowFreeVarUnbounded) {
  LP lp;
  lp.add_column(ColType::Free, -inf, inf, 1);
  lp.add_row(RowType::NonBinding, -inf, inf, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::Unbounded);
  EXPECT_TRUE(is_infinite(splx.get_z()));
  EXPECT_TRUE(is_le(splx.get_z(), 0));
  EXPECT_TRUE(is_infinite(splx.get_x()[0]));
}

TEST(SimplexTest, RangeRowFixedVarOptimalSolution) {
  LP lp;
  lp.add_column(ColType::Fixed, 0, 0, -1);
  lp.add_row(RowType::Range, -5, 5, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), 0));
  EXPECT_TRUE(is_eq(splx.get_x()[0], 0));
}

TEST(SimplexTest, EqualityRowLowerVarsOptimalSolution) {
  LP lp;
  lp.add_column(ColType::LowerBound, 1.0, inf, 2);
  lp.add_column(ColType::LowerBound, 1.0, inf, 1);
  lp.add_row(RowType::Equality, 3, 3, {1, 1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), 4.0));
}

TEST(SimplexTwoPhaseTest, FourRowsFourVarsOptimalSolution) {
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

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -32));
}

TEST(SimplexPhaseOneTest, LowerRowLowerVarInfeasible) {
  LP lp;
  lp.add_column(ColType::LowerBound, 1.0, inf, 2);
  lp.add_row(RowType::LE, -inf, 0, {1});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::Infeasible);
}

TEST(SimplexPhaseOneTest, EqualityRowFixedVarInfeasible) {
  LP lp;
  lp.add_column(ColType::Fixed, -3.0, -3.0, 2);
  lp.add_row(RowType::Equality, 0, 0, {2.5});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::Infeasible);
}

TEST(SimplexTest, ThreeRowsThreeVarsOptimalSolution) {
  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf, -1);
  lp.add_column(ColType::LowerBound, 0, inf, -2);
  lp.add_column(ColType::LowerBound, 0, inf, 3);
  lp.add_row(RowType::LE, -inf, 14, {2, 1, 1});
  lp.add_row(RowType::LE, -inf, 28, {4, 2, 3});
  lp.add_row(RowType::LE, -inf, 30, {2, 5, 5});

  Simplex splx(lp);
  splx.solve();

  EXPECT_TRUE(splx.get_result() == Simplex::Result::OptimalSolution);
  EXPECT_TRUE(is_eq(splx.get_z(), -13));
}
