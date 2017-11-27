#include "pricer.h"
#include "gtest/gtest.h"

namespace fizplex {

class PricerTestTwoVars : public ::testing::Test {
protected:
  LP lp;
  std::vector<size_t> nbi = {0, 1};
  Pricer p;
  void SetUp() {
    lp.add_column(ColType::Bounded, -1.0, 1.0);
    lp.add_column(ColType::Bounded, -1.0, 1.0);
  }
};

TEST_F(PricerTestTwoVars, ZeroReducedCostsIsOptimal) {
  DVector x = {1.0, 1.0};
  DVector d = {0.0, 0.0};
  EXPECT_TRUE(p.price(x, lp, d, nbi).is_optimal);
}

TEST_F(PricerTestTwoVars, NegCostAtLowerIsNotOptimal) {
  DVector x = {1.0, -1.0};
  DVector d = {0.0, -1.0};
  const auto candidate = p.price(x, lp, d, nbi);
  EXPECT_FALSE(candidate.is_optimal);
  EXPECT_EQ(candidate.index, 1u);
}

TEST_F(PricerTestTwoVars, PosCostAtUpperIsNotOptimal) {
  DVector x = {-1.0, 1.0};
  DVector d = {0.0, 1.0};
  const auto candidate = p.price(x, lp, d, nbi);
  EXPECT_FALSE(candidate.is_optimal);
  EXPECT_EQ(candidate.index, 1u);
}

} // namespace fizplex
