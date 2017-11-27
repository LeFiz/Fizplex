#include "colmatrix.h"
#include "gtest/gtest.h"

namespace fizplex {

TEST(ColMatrix, RowAndColCountEmptyMatrix) {
  const ColMatrix m;
  EXPECT_TRUE(m.row_count() == 0);
  EXPECT_TRUE(m.col_count() == 0);
}

TEST(ColMatrix, RowAndColCountNonEmptyMatrix) {
  const ColMatrix m(3, 4);
  EXPECT_TRUE(m.row_count() == 3);
  EXPECT_TRUE(m.col_count() == 4);
}

TEST(ColMatrix, EqualityOperatorsWhenZeroValIsAdded) {
  const ColMatrix m(3, 3,
                    {{{0, 7}, {2, -3}},
                     {{0, 2}, {2, 4}, {1, 3}},
                     {{0, 1}, {1, -1}, {2, -2}}});
  ColMatrix m_with_zero_added = m;
  m_with_zero_added.add_value(1, 0, 0.0);
  EXPECT_TRUE(m == m_with_zero_added);
  EXPECT_TRUE(m_with_zero_added == m);
  EXPECT_FALSE(m != m_with_zero_added);
  EXPECT_FALSE(m_with_zero_added != m);
}

TEST(ColMatrix, EqualityOperatorsSameMatrixButWrongDimensions) {
  const ColMatrix m45(4, 5,
                      {{{0, 7}, {2, -3}},
                       {{0, 2}, {2, 4}, {1, 3}},
                       {{0, 1}, {1, -1}, {2, -2}}});
  const ColMatrix m33(3, 3,
                      {{{0, 7}, {2, -3}},
                       {{0, 2}, {1, 3}, {2, 4}},
                       {{0, 1}, {1, -1}, {2, -2}}});
  EXPECT_FALSE(m45 == m33);
  EXPECT_TRUE(m45 != m33);
}

TEST(ColMatrix, AssignmentOperator) {
  ColMatrix m11(1, 1);
  const ColMatrix m33(3, 3,
                      {{{0, 7}, {2, -3}, {1, 0.0}},
                       {{0, 2}, {1, 3}, {2, 4}},
                       {{0, 1}, {1, -1}, {2, -2}}});
  m11 = m33;
  EXPECT_EQ(m33, m11);
}

TEST(ColMatrix, AddRowToEmptyMatrix) {
  ColMatrix m;
  m.add_row();
  EXPECT_EQ(1u, m.row_count());
}

TEST(ColMatrix, AddColToEmptyMatrix) {
  ColMatrix m;
  m.add_column();
  EXPECT_EQ(1u, m.col_count());
}

TEST(ColMatrix, AddColumnAndValuesToNonEmptyMatrix) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}, {1, 0.0}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  const SVector new_col = {{0, 1.0}, {2, 2.0}, {3, 3.0}};
  m.add_column(new_col);
  EXPECT_EQ(new_col, m.column(3));
}

TEST(ColMatrix, SwapColumns) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  const SVector v0 = m.column(0);
  const SVector v2 = m.column(2);
  m.swap_columns(0, 2);
  EXPECT_EQ(v2, m.column(0));
  EXPECT_EQ(v0, m.column(2));
}

TEST(ColMatrix, IdentityMatrix) {
  auto m = ColMatrix::identity(3);
  EXPECT_EQ(SVector({{1, 1.0}}), m.column(1));
}

TEST(ColMatrix, MatrixMultiplicationWithZero) {
  const auto zero = ColMatrix(3, 3);
  const auto idnt = ColMatrix::identity(3);
  EXPECT_EQ(zero, idnt * zero);
}

TEST(ColMatrix, MatrixMultiplicationWithIdentity) {
  const auto idnt = ColMatrix::identity(3);
  const ColMatrix m(3, 3,
                    {{{0, 7}, {2, -3}, {1, 0.0}},
                     {{0, 2}, {1, 3}, {2, 4}},
                     {{0, 1}, {1, -1}, {2, -2}}});
  EXPECT_EQ(m, idnt * m);
  EXPECT_EQ(m, m * idnt);
}

TEST(ColMatrix, MatrixMultiplicationDifferentDimensions) {
  const ColMatrix a(
      2, 4,
      {{{0, 2}, {1, -7}}, {{0, 3}, {1, 2}}, {{0, -1}, {1, 1}}, {{1, 10}}});
  const ColMatrix b(
      4, 2,
      {{{0, 3}, {1, 2}, {2, -1}, {3, 2}}, {{0, 4}, {1, 1}, {2, 2}, {3, 7}}});
  const ColMatrix res(2, 2, {{{0, 13}, {1, 2}}, {{0, 9}, {1, 46}}});
  EXPECT_EQ(res, a * b);
}

TEST(ColMatrix, MatrixMultiplicationWrongDimensions) {
  const auto a = ColMatrix::identity(3);
  const auto b = ColMatrix::identity(2);
  EXPECT_THROW(a * b, std::invalid_argument);
}

} // namespace fizplex
