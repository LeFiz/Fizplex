#include "dvector.h"
#include "numeric.h"
#include "gtest/gtest.h"

namespace fizplex {

TEST(DVectorDimensionTest, NewVectorHasCorrectDim) {
  EXPECT_TRUE(DVector().dimension() == 0);
  EXPECT_TRUE(DVector(5).dimension() == 5);
}

class DVectorSingleVectorTest : public ::testing::Test {
public:
  DVector vec = {0.0, 1.0, 2.0, 3.0, 4.0};
};

TEST_F(DVectorSingleVectorTest, ResizeToLargerDimLeavesValuesButChangesDim) {
  vec.resize(10);
  EXPECT_EQ(10u, vec.dimension());
  EXPECT_DOUBLE_EQ(2.0, vec[2]);
}

TEST_F(DVectorSingleVectorTest, AppendAddsNewValue) {
  vec.append(5.0);
  EXPECT_DOUBLE_EQ(5.0, vec[5]);
}

TEST_F(DVectorSingleVectorTest, RandomAccessAssignmentSetsValue) {
  vec[2] = 3.14;
  EXPECT_DOUBLE_EQ(3.14, vec[2]);
}

TEST(DVectorDotProductTest, ZeroVectors) {
  EXPECT_TRUE(is_zero(DVector(10) * DVector(10)));
}

TEST(DVectorDotProductTest, NonZeroVectors) {
  const DVector v = {0.5, 0.0, 0.2};
  const DVector w = {2.0, 4.0, 6.0};
  EXPECT_DOUBLE_EQ(2.2, v * w);
  EXPECT_DOUBLE_EQ(v * w, w * v);
}

TEST(DVectorSVectorDotProductTest, ZeroVectors) {
  EXPECT_DOUBLE_EQ(0.0, DVector(5) * SVector());
}

TEST(DVectorSVectorDotProductTest, NonZeroVectors) {
  const DVector v = {2.0, 1.0, 0.5};
  const SVector w = {{0, 0.5}, {2, 2.0}};
  EXPECT_DOUBLE_EQ(2.0, v * w);
}

TEST(DVectorTest, EqualityForEmptyVectors) {
  EXPECT_TRUE(DVector() == DVector());
}

TEST(DVectorTest, EqualityForEqualVectors) {
  const DVector v = {1.0, 2.0, 3.0};
  const DVector w = {1.0, 2.0, 3.0};
  EXPECT_TRUE(v == w);
  EXPECT_FALSE(v != w);
}

TEST(DVectorTest, EqualityForInequalVectors) {
  const DVector v = {1.0, 2.0, 3.0};
  const DVector w = {-1.0, -2.0, -3.0};
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
}

TEST(DVectorTest, MultiplicationWithScalar) {
  EXPECT_EQ(DVector(2), 0.0 * DVector({1.5, 0.5}));
  EXPECT_EQ(DVector({3.0, 1.0}), 2.0 * DVector({1.5, 0.5}));
}

TEST(DVectorTest, OperatorCompoundSubtraction) {
  DVector v = {1.0, 2.0, 3.0};
  v -= DVector({0.0, 1.0, 2.0});
  EXPECT_EQ(DVector({1.0, 1.0, 1.0}), v);
}

TEST(DVectorSVectorTest, OperatorCompoundSubtraction) {
  DVector v = {1.0, 2.0, 3.0};
  const SVector sv = {{0, 0.0}, {1, 1.0}, {2, 2.0}};
  v -= sv;
  EXPECT_EQ(DVector({1.0, 1.0, 1.0}), v);
}

TEST(DVectorTest, MaxAbs) {
  EXPECT_DOUBLE_EQ(0.0, DVector().max_abs());
  EXPECT_DOUBLE_EQ(4.4, DVector({3.1, 4.4, -1.3}).max_abs());
  EXPECT_DOUBLE_EQ(3.1, DVector({-3.1, 3.0}).max_abs());
}

} // namespace fizplex
