#include "svector.h"
#include "gtest/gtest.h"

namespace fizplex {

TEST(SVectorTest, EmptyHasZeolength) { EXPECT_TRUE(SVector().length() == 0); }

TEST(SVectorTest, NonEmptyLength) {
  EXPECT_TRUE(SVector({{0, -2}, {7, -2.11}}).length() == 2);
}

TEST(SVectorTest, AddValueInsertsNewValue) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  const double d = 3.14;
  v.add_value(3, d);
  EXPECT_EQ(5u, v.length());
  EXPECT_DOUBLE_EQ(d, v.get_value(3));
}

TEST(SVectorTest, AddValueCanAddSameIndexTwice) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  const double d = 3;
  v.add_value(1, d);
  EXPECT_TRUE(v.length() == 5);
}

TEST(SVectorTest, ReorderedVectorsAreEqual) {
  const SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  const SVector w = {{2, 9}, {0, -2}, {7, -2.11}, {1, 3}};
  EXPECT_TRUE(v == w);
  EXPECT_TRUE(w == v);
  EXPECT_TRUE(!(v != w));
  EXPECT_TRUE(!(w != v));
}

TEST(SVectorTest, NonEmptyVecIsNotEqualToEmpty) {
  const SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  const SVector w;
  EXPECT_TRUE(!(v == w));
  EXPECT_TRUE(v != w);
}

TEST(SVectorTest, ChangeValsThroughIteratorSetsRightVal) {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.11}};
  for (auto &n : v)
    n.value *= 3;
  EXPECT_DOUBLE_EQ(27.0, v.get_value(2));
}

TEST(SVectorTest, ScalaMult) {
  EXPECT_TRUE(2.0 * SVector({{0, -2}, {4, 9}}) == SVector({{0, -4}, {4, 18}}));
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

} // namespace fizplex
