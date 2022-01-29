#include "simplex.h"
#include "gtest/gtest.h"

namespace fizplex {

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

TEST_F(SimplexTest, MySimplexExp) {
    lp.add_column(ColType::LowerBound, 0, inf, -1); // x0 >= 0
    lp.add_column(ColType::LowerBound, 0, inf, -1); // x1 >= 0
                                                    // -x0 - x1 -> min
    lp.add_row(RowType::LE, -inf, 10, {1, 0});      // x0 + .. <= 10
    lp.add_row(RowType::LE, -inf, 20, {0, 1});      // .. + x1 <= 20
    
    solve_lp();
    EXPECT_EQ(Simplex::Result::OptimalSolution, result);
    EXPECT_DOUBLE_EQ(-30.0, z);
    EXPECT_DOUBLE_EQ(10.0, x[0]);
    EXPECT_DOUBLE_EQ(20.0, x[1]);
}


TEST_F(SimplexTest, RealSimplexExp) {
    lp.add_column(ColType::LowerBound, 0, inf, -13);    // x0 >= 0
    lp.add_column(ColType::LowerBound, 0, inf, -25);    // x1 >= 0
    lp.add_column(ColType::LowerBound, 0, inf, -52);    // x2 >= 0
                                                        // -13x0 - 25x1 - 52x2 -> min
    lp.add_row(RowType::LE, -inf, 10, {1.2, 2.0, 3.1}); // 1.2*x0 + 2*x1 + 3.1*x2 <= 20
    lp.add_row(RowType::LE, -inf, 450, {30, 90, 250});  // 30*x0  + 90*x1 + 250*x2 <= 250
    
    lp.add_row(RowType::LE, -inf, 4, {1, 0, 0});        // x0 <= 5
//    lp.add_row(RowType::GE, 5, inf, {1, 0, 0});        // x0 >= 6
    
//    lp.add_row(RowType::LE, -inf, 0, {0, 1, 0});        // x1 <=0
    lp.add_row(RowType::GE, 1, inf, {0, 1, 0});        // x1 >=1
    
    lp.add_row(RowType::LE, -inf, 0, {0, 0, 1});        // x2 <= 1
//    lp.add_row(RowType::GE, 1, inf, {0, 0, 1});        // x2 >= 2 –– infeasible
    
    solve_lp();
    EXPECT_EQ(Simplex::Result::OptimalSolution, result);
    printf("result : %i\n", result);
    printf("obj value: %f\n", -z);
    printf("solution: %f %f %f\n", x[0], x[1], x[2]);
}

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
  lp.add_column(ColType::Bounded, 0, 8, -1);
  lp.add_row(RowType::LE, -inf, 14, {2});

  solve_lp();
  EXPECT_EQ(Simplex::Result::OptimalSolution, result);
  EXPECT_DOUBLE_EQ(-7.0, z);
    EXPECT_DOUBLE_EQ(7.0, x[0]);
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

} // namespace fizplex
