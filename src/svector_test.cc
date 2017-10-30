#include "svector.h"
#include "gtest/gtest.h"

TEST(SVectorTest, EmptyHasZeolength) {
  SVector v;
  EXPECT_TRUE(v.length() == 0);
}

TEST(SVectorTest, NonEmptyLength) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  EXPECT_TRUE(v.length() == 4);
}

TEST(SVectorTest, AddValueInsertsNewValue) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  const double d = 3.14;
  v.add_value(3, d);
  EXPECT_TRUE(v.length() == 5);
  EXPECT_TRUE(is_eq(v.get_value(3), d));
}

TEST(SVectorTest, AddValueCanAddSameValueTwice) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  const double d = 3;
  v.add_value(1, d);
  EXPECT_TRUE(v.length() == 5);
}

TEST(SVectorTest, ReorderedVectorsAreEqual) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  SVector w = {{2, 9}, {0, -2}, {7, -2.11}, {1, 3}};
  EXPECT_TRUE(v == w);
  EXPECT_TRUE(w == v);
  EXPECT_TRUE(!(v != w));
  EXPECT_TRUE(!(w != v));
}

TEST(SVectorTest, NonEmptyVecIsNotEqualToEmpty) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  SVector w;
  EXPECT_TRUE(!(v == w));
  EXPECT_TRUE(v != w);
}

TEST(SVectorTest, ChangeValsThroughIteratorSetsRightVal) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  for (auto &n : v) {
    n.value *= 3;
  }
  EXPECT_TRUE(is_eq(v.get_value(2), 27));
}

TEST(SVectorTest, CanIterateEmptyVec) {
  SVector v;
  for (auto &n : v) {
    n.value *= 3;
    EXPECT_TRUE(false);
  }
}

TEST(SVectorTest, ScalaMult) {
  SVector v = {{0, -2}, {1, 3}, {4, 9}};
  SVector w = {{0, -4}, {1, 6}, {4, 18}};
  EXPECT_TRUE(2.0 * v == w);
}

TEST(SVectorTest, SubscriptUpdatesExistingValues) {
  SVector v = {{0, -2.1}, {3, 0.4}};
  v[0] = 3.0;
  EXPECT_DOUBLE_EQ(3.0, v[0]);
}

TEST(SVectorTest, SubscriptAddsNewValues) {
  SVector v = {{0, -2.1}, {3, 0.4}};
  v[5] = 3.2;
  EXPECT_DOUBLE_EQ(3.2, v[5]);
  EXPECT_DOUBLE_EQ(0.4, v[3]);
}
