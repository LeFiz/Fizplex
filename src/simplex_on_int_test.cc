#include "simplex_on_integers.h"
#include "gtest/gtest.h"

namespace fizplex {

class SimplexOnIntegerTest : public ::testing::Test {
public:
  void solve_lp() {
    SimplexOnIntegers splx(lp);
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

TEST_F(SimplexOnIntegerTest, SimplexOnIntegersBasic) {
    lp.add_column(ColType::LowerBound, 0, inf, -1); // x0 >= 0
    lp.add_column(ColType::LowerBound, 0, inf, -1); // x1 >= 0
                                                    // -x0 - x1 -> min
    lp.add_row(RowType::LE, -inf, 10.5, {1, 0});      // x0 + .. <= 10.5
    lp.add_row(RowType::LE, -inf, 20.5, {0, 1});      // .. + x1 <= 20.5

    solve_lp();
    EXPECT_EQ(Simplex::Result::OptimalSolution, result);
    EXPECT_DOUBLE_EQ(30.0, z);
    EXPECT_DOUBLE_EQ(10.0, x[0]);
    EXPECT_DOUBLE_EQ(20.0, x[1]);
}

TEST_F(SimplexOnIntegerTest, SimplexOnIntegersInfeasible) {
    lp.add_column(ColType::LowerBound, 0.1, inf, -1); // x0 >= 0
    lp.add_column(ColType::LowerBound, 0, inf, -1); // x1 >= 0
                                                    // -x0 - x1 -> min
    lp.add_row(RowType::LE, -inf, 0.9, {1, 0});      // x0 + .. <= 0.9
    lp.add_row(RowType::LE, -inf, 20.5, {0, 1});      // .. + x1 <= 20.5

    solve_lp();
    EXPECT_EQ(Simplex::Result::Infeasible, result);
}



} // namespace fizplex
