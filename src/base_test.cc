#include "base.h"
#include "gtest/gtest.h"

TEST(BaseTestSingular, InvertIsFalse) {
  const ColMatrix m(3, 3,
                    {{{0, 1}, {1, 1}, {2, 1}},
                     {{0, 1}, {1, 1}, {2, 1}},
                     {{0, 1}, {1, 1}, {2, 1}}});
  EXPECT_FALSE(Base(m).invert());
}

class BaseTestRegularNoReordering : public ::testing::Test {
public:
  BaseTestRegularNoReordering()
      : b(ColMatrix(3, 3,
                    {{{0, 7}, {2, -3}},
                     {{0, 2}, {1, 3}, {2, 4}},
                     {{0, 1}, {1, -1}, {2, -2}}})) {
    is_invertible = b.invert();
  };

  Base b;
  bool is_invertible;
  SVector sv0 = {{0, 1}};
  SVector sv1 = {{1, 1}};
  SVector sv2 = {{2, 1}};
  DVector dv0 = {1, 0, 0};
  DVector dv1 = {0, 1, 0};
  DVector dv2 = {0, 0, 1};
};

TEST_F(BaseTestRegularNoReordering, IsInvertible) {
  EXPECT_TRUE(is_invertible);
}

TEST_F(BaseTestRegularNoReordering, Invert) {
  b.ftran(sv0);
  b.ftran(sv1);
  b.ftran(sv2);
  EXPECT_EQ(SVector({{0, -2}, {1, 3}, {2, 9}}), sv0);
  EXPECT_EQ(SVector({{0, 8}, {1, -11}, {2, -34}}), sv1);
  EXPECT_EQ(SVector({{0, -5}, {1, 7}, {2, 21}}), sv2);
}

TEST_F(BaseTestRegularNoReordering, ForwardTransformDVector) {
  b.ftran(dv0);
  b.ftran(dv1);
  b.ftran(dv2);
  EXPECT_EQ(DVector({-2, 3, 9}), dv0);
  EXPECT_EQ(DVector({8, -11, -34}), dv1);
  EXPECT_EQ(DVector({-5, 7, 21}), dv2);
}

TEST(BaseDiagonalMatrixTest, ExplicitInverse) {
  const ColMatrix m(2, 2, {{{0, 2.0}}, {{1, 3.0}}});
  const ColMatrix m_minus_one(2, 2, {{{0, 0.5}}, {{1, 1.0 / 3.0}}});
  Base b(m);
  b.invert();
  EXPECT_EQ(m_minus_one, b.get_inverse());
}

TEST_F(BaseTestRegularNoReordering, ExplicitInverse) {
  const ColMatrix inv(3, 3,
                      {{{0, -2}, {1, 3}, {2, 9}},
                       {{0, 8}, {1, -11}, {2, -34}},
                       {{0, -5}, {1, 7}, {2, 21}}});
  EXPECT_EQ(inv, b.get_inverse());
}

TEST_F(BaseTestRegularNoReordering, BackwardTransformDVector) {
  b.btran(dv0);
  b.btran(dv1);
  b.btran(dv2);
  EXPECT_EQ(DVector({-2, 8, -5}), dv0);
  EXPECT_EQ(DVector({3, -11, 7}), dv1);
  EXPECT_EQ(DVector({9, -34, 21}), dv2);
}

class BaseTestRegularWithReordering : public ::testing::Test {
public:
  BaseTestRegularWithReordering() : b(m) { is_invertible = b.invert(); };
  const ColMatrix m = ColMatrix(3, 3,
                                {{{1, 7}, {2, -3}},
                                 {{0, 2}, {1, 3}, {2, 4}},
                                 {{0, 1}, {1, -1}, {2, -2}}});
  Base b;
  bool is_invertible;
  SVector sv0 = {{0, 1}};
  SVector sv1 = {{1, 1}};
  SVector sv2 = {{2, 1}};
  DVector dv0 = {1, 0, 0};
  DVector dv1 = {0, 1, 0};
  DVector dv2 = {0, 0, 1};
};

TEST_F(BaseTestRegularWithReordering, IsInvertible) { EXPECT_TRUE(b.invert()); }

TEST_F(BaseTestRegularWithReordering, ForwardTransformSVector) {
  b.ftran(sv0);
  b.ftran(sv1);
  b.ftran(sv2);
  EXPECT_EQ(SVector({{0, -2.0 / 71}, {1, 17.0 / 71}, {2, 37.0 / 71}}), sv0);
  EXPECT_EQ(SVector({{0, 8.0 / 71}, {1, 3.0 / 71}, {2, -6.0 / 71}}), sv1);
  EXPECT_EQ(SVector({{0, -5.0 / 71}, {1, 7.0 / 71}, {2, -14.0 / 71}}), sv2);
}

TEST_F(BaseTestRegularWithReordering, ExplicitInverse) {
  const ColMatrix inv(3, 3,
                      {{{0, -2.0 / 71}, {1, 17.0 / 71}, {2, 37.0 / 71}},
                       {{0, 8.0 / 71}, {1, 3.0 / 71}, {2, -6.0 / 71}},
                       {{0, -5.0 / 71}, {1, 7.0 / 71}, {2, -14.0 / 71}}});
  EXPECT_EQ(inv, b.get_inverse());
}

TEST_F(BaseTestRegularWithReordering, ForwardTransformDVector) {
  b.ftran(dv0);
  b.ftran(dv1);
  b.ftran(dv2);
  EXPECT_EQ(DVector({-2.0 / 71, 17.0 / 71, 37.0 / 71}), dv0);
  EXPECT_EQ(DVector({8.0 / 71, 3.0 / 71, -6.0 / 71}), dv1);
  EXPECT_EQ(DVector({-5.0 / 71, 7.0 / 71, -14.0 / 71}), dv2);
}

TEST_F(BaseTestRegularWithReordering, BackwardTransformDVector) {
  b.btran(dv0);
  b.btran(dv1);
  b.btran(dv2);
  EXPECT_EQ(DVector({-2.0 / 71, 8.0 / 71, -5.0 / 71}), dv0);
  EXPECT_EQ(DVector({17.0 / 71, 3.0 / 71, 7.0 / 71}), dv1);
  EXPECT_EQ(DVector({37.0 / 71, -6.0 / 71, -14.0 / 71}), dv2);
}

TEST_F(BaseTestRegularWithReordering, BaseMatrixTimesInverseIsIdentity) {
  const auto inv = b.get_inverse();
  EXPECT_EQ(ColMatrix::identity(3), inv * m);
  EXPECT_EQ(ColMatrix::identity(3), m * inv);
}
