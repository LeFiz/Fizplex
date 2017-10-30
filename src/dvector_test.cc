#include "dvector.h"
#include "numeric.h"
#include "gtest/gtest.h"

TEST(DVectorTest, NewVectorHasCorrectDim) {
  DVector v(5);
  EXPECT_TRUE(v.dimension() == 5);
}

TEST(DVectorTest, ResizeToLargerDimLeavesValuesButChangesDim) {
  DVector v(5);
  const double d = 3.14;
  v[0] = d;
  v.resize(10);
  EXPECT_TRUE(v.dimension() == 10);
  EXPECT_TRUE(is_eq(v[0], d));
}

TEST(DVectorTest, ResizeToSmallerDimLeavesValuesButChangesDim) {
  DVector v(5);
  const double d = 3.14;
  v[0] = d;
  v.resize(1);
  EXPECT_TRUE(v.dimension() == 1);
  EXPECT_TRUE(is_eq(v[0], d));
}

TEST(DVectorTest, AppendAddsNewValue) {
  DVector v(5);
  const double d = 1.23;
  v.append(d);
  EXPECT_TRUE(is_eq(v[5], d));
}

TEST(DVectorTest, NonConstMemberAccess) {
  DVector v(5);
  double d = 3.1;
  v[2] = d;
  EXPECT_TRUE(is_eq(v[2], d));
}

TEST(DVectorDotProductTest, ZeroVectors) {
  DVector v(10), w(10);
  EXPECT_TRUE(is_zero(v * w));
}

TEST(DVectorDotProductTest, NonZeroVectors) {
  DVector v(10), w(10);
  const double d = 7.123456;
  v[3] = 1;
  w[3] = d;
  v[7] = 0.5;
  w[7] = 2.0;
  EXPECT_TRUE(is_eq(v * w, d + 1));
  EXPECT_TRUE(is_eq(v * w, w * v));
}

TEST(DVectorSVectorDotProductTest, ZeroVectors) {
  DVector v(10);
  SVector w;
  EXPECT_TRUE(is_zero(v * w));
}

TEST(DVectorSVectorDotProductTest, NonZeroVectors) {
  DVector v(10);
  SVector w;
  const double d = 7.123456;
  v[3] = 1;
  w.add_value(3, d);
  v[7] = 0.5f;
  w.add_value(7, 2.0);
  EXPECT_TRUE(is_eq(v * w, d + 1));
}

TEST(DVectorTest, EqualityForEmptyVectors) {
  DVector v, w;
  EXPECT_TRUE(v == w);
}

TEST(DVectorTest, EqualityForEqualVectors) {
  DVector v(3), w(3);
  const double d = 2.123456;
  v[0] = 1;
  w[0] = 1;
  v[2] = d;
  w[2] = d;
  EXPECT_TRUE(v == w);
  EXPECT_TRUE(!(v != w));
}

TEST(DVectorTest, EqualityForInequalVectors) {
  DVector v(3), w(3);
  const double d = 2.123456;
  v[0] = 1;
  w[0] = 1;
  v[1] = 17;
  v[2] = d;
  w[2] = d;
  EXPECT_TRUE(!(v == w));
  EXPECT_TRUE((v != w));
}

TEST(DVectorTest, MultiplicationWithScalar) {
  DVector v(3), w(3);
  v[0] = 1;
  w[0] = 2;
  v[1] = 17.5;
  w[1] = 35.0;
  v[2] = 0;
  w[2] = 0;
  EXPECT_TRUE(2.0f * v == w);
}

TEST(DVectorTest, OperatorCompoundSubtraction) {
  DVector v(3), w(3);
  v[0] = 1;
  w[0] = 2;
  v[1] = 17.5;
  w[1] = 35.0;
  v[2] = 0;
  w[2] = 0;
  w -= v;
  EXPECT_TRUE(v == w);
}

TEST(DVectorSVectorTest, OperatorCompoundSubtraction) {
  DVector v(3), w(3);
  v[0] = 4;
  v[1] = 40;
  v[2] = 0.5;
  w[0] = 2;
  w[1] = 35.0;
  w[2] = 0.75;

  SVector sv = {{0, 2}, {1, 5}, {2, -0.25}};
  v -= sv;
  EXPECT_TRUE(v == w);
}

TEST(DVectorTest, MaxAbs) {
  EXPECT_DOUBLE_EQ(0.0, DVector().max_abs());
  EXPECT_DOUBLE_EQ(4.4, DVector({3.1, 4.4, -1.3}).max_abs());
  EXPECT_DOUBLE_EQ(3.1, DVector({-3.1, 3.0}).max_abs());
}
