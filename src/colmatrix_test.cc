
#include "colmatrix.h"
#include "gtest/gtest.h"

TEST(ColMatrix, RowCountEmptyMatrix) {
  ColMatrix m;
  EXPECT_TRUE(m.row_count() == 0);
}

TEST(ColMatrix, RowCountNonEmptyMatrix) {
  ColMatrix m(3, 4);
  EXPECT_TRUE(m.row_count() == 3);
}

TEST(ColMatrix, ColCountEmptyMatrix) {
  ColMatrix m;
  EXPECT_TRUE(m.col_count() == 0);
}

TEST(ColMatrix, ColCountNonEmptyMatrix) {
  ColMatrix m(3, 4);
  EXPECT_TRUE(m.col_count() == 4);
}

TEST(ColMatrix, EqualityOperatorForEqualMatrices) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {2, 4}, {1, 3}},
               {{0, 1}, {1, -1}, {2, -2}}});
  ColMatrix mm(3, 3,
               {{{0, 7}, {2, -3}, {1, 0.0}},
                {{0, 2}, {1, 3}, {2, 4}},
                {{0, 1}, {1, -1}, {2, -2}}});
  EXPECT_TRUE(m == mm);
  EXPECT_TRUE(mm == m);
  EXPECT_TRUE(!(m != mm));
  EXPECT_TRUE(!(mm != m));
}

TEST(ColMatrix, EqualityOperatorWrongDimensions) {
  ColMatrix m(4, 5,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {2, 4}, {1, 3}},
               {{0, 1}, {1, -1}, {2, -2}}});
  ColMatrix mm(3, 3,
               {{{0, 7}, {2, -3}, {1, 0.0}},
                {{0, 2}, {1, 3}, {2, 4}},
                {{0, 1}, {1, -1}, {2, -2}}});
  EXPECT_TRUE(!(m == mm));
  EXPECT_TRUE(!(mm == m));
  EXPECT_TRUE(m != mm);
  EXPECT_TRUE(mm != m);
}

TEST(ColMatrix, AssignmentOperator) {
  ColMatrix m(1, 1);
  ColMatrix mm(3, 3,
               {{{0, 7}, {2, -3}, {1, 0.0}},
                {{0, 2}, {1, 3}, {2, 4}},
                {{0, 1}, {1, -1}, {2, -2}}});
  m = mm;
  EXPECT_TRUE(m == mm);
}

TEST(ColMatrix, AddRowToEmptyMatrix) {
  ColMatrix m;
  m.add_row();
  EXPECT_TRUE(m.row_count() == 1);
}

TEST(ColMatrix, AddColToEmptyMatrix) {
  ColMatrix m;
  m.add_column();
  EXPECT_TRUE(m.col_count() == 1);
}

TEST(ColMatrix, AddColumnAndValuesToNonEmptyMatrix) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}, {1, 0.0}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  m.add_column();
  const double d = 2.111;
  m.add_value(1, 3, d);
  SVector v = {{1, d}};
  EXPECT_TRUE(v == m.column(3));
}

TEST(ColMatrix, GetColumn) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  SVector v = {{0, 2}, {1, 3}, {2, 4}};
  EXPECT_TRUE(m.column(1) == v);
}

TEST(ColMatrix, SwapColumns) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  SVector v0 = m.column(0);
  SVector v2 = m.column(2);
  m.swap_columns(0, 2);
  EXPECT_TRUE(m.column(0) == v2);
  EXPECT_TRUE(m.column(2) == v0);
}
