#include "numeric.h"
#include "gtest/gtest.h"

TEST(NumericTest, Equality) {
  EXPECT_TRUE(is_eq(2.0, 2));
  EXPECT_TRUE(!is_eq(2.0, 3));
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

TEST(NumericTest, StrictlyGreater) {
  EXPECT_TRUE(!is_greater(-1.0, 1.0));
  EXPECT_TRUE(is_greater(1.0, 0.0));
  EXPECT_TRUE(!is_greater(1.0, 1.0));
  EXPECT_TRUE(is_greater(21.0, -9));
}
