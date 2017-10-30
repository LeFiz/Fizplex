#include "rhs_scaler.h"
#include "gtest/gtest.h"

TEST(RhsScalerTest, AlreadyScaledVectorIsUnchanged) {
  DVector v = {1.0, -1.0, 0.5, -0.5};
  RhsScaler rs(v.max_abs());
  rs.scale(v);
  EXPECT_EQ(v, DVector({1.0, -1.0, 0.5, -0.5}));
  rs.unscale(v);
  EXPECT_EQ(v, DVector({1.0, -1.0, 0.5, -0.5}));
}

TEST(RhsScalerTest, LargerVectorIsChanged) {
  DVector v = {6.0, -3.0};
  RhsScaler rs(v.max_abs());
  rs.scale(v);
  EXPECT_EQ(v, DVector({1.0, -0.5}));
  rs.unscale(v);
  EXPECT_EQ(v, DVector({6.0, -3.0}));
}

TEST(RhsScalerTest, VectorNearZeroIsChanged) {
  DVector v = {0.5, -0.01};
  RhsScaler rs(v.max_abs());
  rs.scale(v);
  EXPECT_EQ(v, DVector({1.0, -0.02}));
  rs.unscale(v);
  EXPECT_EQ(v, DVector({0.5, -0.01}));
}

TEST(RhsScalerTest, ScalarScaling) {
  DVector v = {6.0, -3.0};
  RhsScaler rs(v.max_abs());
  double d = -18.6;
  rs.scale(d);
  EXPECT_DOUBLE_EQ(-3.1, d);
  rs.unscale(d);
  EXPECT_DOUBLE_EQ(-18.6, d);
}
