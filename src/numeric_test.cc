#include "numeric.h"
#include "gtest/gtest.h"

TEST(NumericTest, Equality) {
  EXPECT_TRUE(is_eq(2.0, 2));
  EXPECT_TRUE(!is_eq(2.0, 3));
}

TEST(NumericTest, NormEquality) {
  EXPECT_TRUE(is_eq_norm(2.0, 2));
  EXPECT_TRUE(is_eq_norm(0.00000001, 0.000000011));
  EXPECT_TRUE(is_eq_norm(2000000.01, 2000000.011));
  EXPECT_TRUE(!is_eq_norm(2.0, 3));
}

TEST(NumericTest, IsFinite) {
  EXPECT_TRUE(is_finite(31.0));
  EXPECT_TRUE(is_finite(-31.0));
  EXPECT_TRUE(!is_finite(inf));
  EXPECT_TRUE(!is_finite(-inf));
}

TEST(NumericTest, IsInfinite) {
  EXPECT_TRUE(is_infinite(inf));
  EXPECT_TRUE(is_infinite(-inf));
  EXPECT_TRUE(!is_infinite(31.0));
  EXPECT_TRUE(!is_infinite(-31.0));
}

TEST(NumericTest, GreaterOrEqual) {
  EXPECT_TRUE(is_ge(1.0, 0.0));
  EXPECT_TRUE(is_ge(0.0, 0.0));
  EXPECT_TRUE(is_ge(-2.1, -9));
}

TEST(NumericTest, LowerOrEqual) {
  EXPECT_TRUE(is_le(-1.0, 0.0));
  EXPECT_TRUE(is_le(0.0, 0.0));
  EXPECT_TRUE(is_le(-30.0, -9));
}

TEST(NumericTest, StrictlyLower) {
  EXPECT_TRUE(is_lower(-1.0, 1.0));
  EXPECT_TRUE(!is_lower(0.0, 0.0));
  EXPECT_TRUE(is_lower(-21.0, -9));
}

TEST(NumericTest, StrictlyLowerNorm) {
  EXPECT_TRUE(is_lower_norm(-1.0, 1.0));
  EXPECT_FALSE(is_lower_norm(1000000.001, 1000000.0011));
  EXPECT_FALSE(is_lower_norm(0.0, 0.0));
}

TEST(NumericTest, StrictlyGreater) {
  EXPECT_TRUE(!is_greater(-1.0, 1.0));
  EXPECT_TRUE(is_greater(1.0, 0.0));
  EXPECT_TRUE(!is_greater(1.0, 1.0));
  EXPECT_TRUE(is_greater(21.0, -9));
}

TEST(NumericTest, StrictlyGreaterNorm) {
  EXPECT_TRUE(!is_greater_norm(-1.0, 1.0));
  EXPECT_TRUE(is_greater_norm(1.0, 0.0));
  EXPECT_TRUE(!is_greater_norm(1.0, 1.0));
  EXPECT_TRUE(is_greater_norm(21.0, -9));
  EXPECT_FALSE(is_greater_norm(1000000.0011, 1000000.001));
}
