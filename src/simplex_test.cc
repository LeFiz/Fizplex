#include "simplex.h"
#include "gtest/gtest.h"

class SimplexTest : public ::testing::Test {
public:
  void solve_lp() {
    Simplex splx(lp);
    splx.solve();
    x = splx.get_x();
    z = splx.get_z();
    result = splx.get_result();
  }
  LP lp;
  DVector x;
  Simplex::Result result;
  double z;
};

TEST_F(SimplexTest, LowerRowLowerVarUnbounded) {
  lp.add_column(ColType::LowerBound, 0, inf, -1);
  lp.add_row(RowType::LE, -inf, 14, {-2});

  solve_lp();
  EXPECT_EQ(Simplex::Result::Unbounded, result);
  EXPECT_TRUE(is_infinite(z));
}

TEST_F(SimplexTest, LowerRowUpperVarOptimalSolution) {
  lp.add_column(ColType::UpperBound, -inf, 0.0, 1);
  lp.add_row(RowType::LE, -inf, 14, {-2});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-7.0, z);
}

TEST_F(SimplexTest, LowerRowBoundedVarOptimalSolutionWithBoundFlip) {
  lp.add_column(ColType::Bounded, 0, 5, -1);
  lp.add_row(RowType::LE, -inf, 14, {2});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-5.0, z);
}

TEST_F(SimplexTest, EqualityRowBoundedVarsOptimalSolution) {
  lp.add_column(ColType::Bounded, 0, 5, -1);
  lp.add_column(ColType::Bounded, 0, 5, 0);
  lp.add_row(RowType::Equality, 0, 0, {1, -1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-5.0, z);
  EXPECT_EQ(DVector({5.0, 5.0, 0.0}), x);
}

TEST_F(SimplexTest, GreaterRowUpperVarOptimalSolution) {
  lp.add_column(ColType::UpperBound, -inf, 0, 1);
  lp.add_row(RowType::GE, -5, inf, {1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-5.0, z);
}

TEST_F(SimplexTest, RangeRowBoundedVarOptimalSolution) {
  lp.add_column(ColType::Bounded, -4, 6, -5);
  lp.add_row(RowType::Range, -5, 5, {1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-25.0, z);
  EXPECT_EQ(DVector({5.0, 0.0}), x);
}

TEST_F(SimplexTest, NonBindingRowFreeVarUnbounded) {
  lp.add_column(ColType::Free, -inf, inf, 1);
  lp.add_row(RowType::NonBinding, -inf, inf, {1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::Unbounded, result);
  EXPECT_TRUE(is_infinite(z));
  EXPECT_TRUE(is_le(z, 0));
  EXPECT_TRUE(is_infinite(x[0]));
}

TEST_F(SimplexTest, RangeRowFixedVarOptimalSolution) {
  lp.add_column(ColType::Fixed, 0, 0, -1);
  lp.add_row(RowType::Range, -5, 5, {1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(0.0, z);
  EXPECT_DOUBLE_EQ(0.0, x[0]);
}

TEST_F(SimplexTest, EqualityRowLowerVarsOptimalSolution) {
  lp.add_column(ColType::LowerBound, 1.0, inf, 2);
  lp.add_column(ColType::LowerBound, 1.0, inf, 1);
  lp.add_row(RowType::Equality, 3, 3, {1, 1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(4.0, z);
}

TEST_F(SimplexTest, FourRowsFourVarsOptimalSolution) {
  lp.add_column(ColType::Free, -inf, inf, 3);
  lp.add_column(ColType::Free, -inf, inf, -2);
  lp.add_column(ColType::LowerBound, 0.0, inf, 1);
  lp.add_column(ColType::LowerBound, 0.0, inf, -4);
  lp.add_row(RowType::GE, 4, inf, {1, 1, -4, 2});
  lp.add_row(RowType::LE, -inf, 6, {-3, 1, -2, 0});
  lp.add_row(RowType::Equality, -1, -1, {0, 1, 0, -1});
  lp.add_row(RowType::Equality, 0, 0, {1, 1, -1, 0});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_TRUE(is_eq_norm(-32.0, z));
}

TEST_F(SimplexTest, LowerRowLowerVarInfeasible) {
  lp.add_column(ColType::LowerBound, 1.0, inf, 2);
  lp.add_row(RowType::LE, -inf, 0, {1});

  solve_lp();
  EXPECT_EQ(Simplex::Result::Infeasible, result);
}

TEST_F(SimplexTest, EqualityRowFixedVarInfeasible) {
  lp.add_column(ColType::Fixed, -3.0, -3.0, 2);
  lp.add_row(RowType::Equality, 0, 0, {2.5});

  solve_lp();
  EXPECT_EQ(Simplex::Result::Infeasible, result);
}

TEST_F(SimplexTest, ThreeRowsThreeVarsOptimalSolution) {
  lp.add_column(ColType::LowerBound, 0, inf, -1);
  lp.add_column(ColType::LowerBound, 0, inf, -2);
  lp.add_column(ColType::LowerBound, 0, inf, 3);
  lp.add_row(RowType::LE, -inf, 14, {2, 1, 1});
  lp.add_row(RowType::LE, -inf, 28, {4, 2, 3});
  lp.add_row(RowType::LE, -inf, 30, {2, 5, 5});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-13.0, z);
}
