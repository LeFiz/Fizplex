#include "ratio_tester.h"
#include "gtest/gtest.h"

namespace fizplex {

class RatioTesterBounded : public ::testing::Test {
protected:
  SVector alpha;
  const DVector beta = {-1.0, -1.0};
  std::vector<size_t> basic = {0};
  LP lp;
  size_t cind = 1u;
  const double ccost = 1.0;
  RatioTester rt;
  void SetUp() {
    lp.add_column(ColType::Bounded, -1.0, 1.0);
    lp.add_column(ColType::LowerBound, -1.0, inf);
  }
};

TEST_F(RatioTesterBounded, AlphaZeroAndNoUpperIsUnbounded) {
  auto res = rt.ratio_test(lp, alpha, beta, cind, basic, ccost);
  EXPECT_EQ(Simplex::IterationDecision::Unbounded, res.result);
}

TEST_F(RatioTesterBounded, AlphaZeroAndUpperIsBoundFlip) {
  basic[0] = 1;
  cind = 0;
  auto res = rt.ratio_test(lp, alpha, beta, cind, basic, ccost);
  EXPECT_EQ(Simplex::IterationDecision::BoundFlip, res.result);
}

TEST_F(RatioTesterBounded, AlphaNonZeroAndNoUpperIsBaseChange) {
  alpha.add_value(0, 1.0);
  auto res = rt.ratio_test(lp, alpha, beta, cind, basic, ccost);
  EXPECT_EQ(Simplex::IterationDecision::BaseChange, res.result);
}

} // namespace fizplex
