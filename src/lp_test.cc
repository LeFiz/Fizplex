#include "lp.h"
#include "gtest/gtest.h"

TEST(LPTest, ColumnCount) {
  LP lp;
  EXPECT_TRUE(lp.column_count() == 0);
}

TEST(LPTest, RowCount) {
  LP lp;
  EXPECT_TRUE(lp.row_count() == 0);
}

TEST(LPTest, AddColumn) {
  LP lp;
  lp.add_column(ColType::Bounded, -1.33333, 302);
  EXPECT_TRUE(lp.column_count() == 1);
}

TEST(LPTest, AddRow) {
  LP lp;
  lp.add_row(RowType::Range, -1.33333, 302);
  EXPECT_TRUE(lp.row_count() == 1);
}

TEST(LPTest, AddRangeRowBIsSetProperly) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  EXPECT_TRUE(is_eq(lp.b[0], 15));
}

TEST(LPTest, AddGERowBIsSetProperly) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::GE, 3, 15);
  EXPECT_TRUE(is_eq(lp.b[0], 3));
}

TEST(LPTest, GetValue) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  const double d = 3.14;
  lp.add_value(0, 0, d);
  EXPECT_TRUE(is_eq(lp.get_value(0, 0), d));
}

TEST(LPTest, GetValueWhenNoValueWasAddedIsZero) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  EXPECT_TRUE(is_zero(lp.get_value(0, 0)));
}

TEST(LPTest, AddObjValue) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  const double d = 3.14;
  lp.add_logicals();
  lp.add_obj_value(0, d);
  EXPECT_TRUE(is_eq(lp.get_obj_value(0), d));
}

TEST(LPTest, AddLogicals) {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Equality, 1, 1);
  lp.add_row(RowType::Range, 3, 15);
  lp.add_row(RowType::LE, -inf, 7);
  lp.add_row(RowType::NonBinding, -inf, inf);
  lp.add_row(RowType::GE, 3, inf);
  lp.add_logicals();

  EXPECT_TRUE(lp.column_count() == 6);

  EXPECT_TRUE(lp.column_header(1).type == ColType::Fixed);
  EXPECT_TRUE(lp.column_header(2).type == ColType::Bounded);
  EXPECT_TRUE(lp.column_header(3).type == ColType::LowerBound);
  EXPECT_TRUE(lp.column_header(4).type == ColType::Free);
  EXPECT_TRUE(lp.column_header(5).type == ColType::UpperBound);

  EXPECT_TRUE(is_eq(lp.column_header(1).lower, 0));
  EXPECT_TRUE(is_eq(lp.column_header(1).upper, 0));
  EXPECT_TRUE(is_eq(lp.column_header(2).lower, 0));
  EXPECT_TRUE(is_eq(lp.column_header(2).upper, 12));
  EXPECT_TRUE(is_eq(lp.column_header(3).lower, 0));
  EXPECT_TRUE(is_eq(lp.column_header(5).upper, 0));

  for (int i = 0; i < 5; i++)
    EXPECT_TRUE(is_eq(lp.get_value(i, 1 + i), 1));
}

TEST(LPTest, IsFeasibleForBoundedVarAndRangeRow) {
  LP lp;
  lp.add_column(ColType::Bounded, -1.0f, 5);
  lp.add_row(RowType::Range, 0, 7);
  lp.add_logicals();
  lp.add_value(0, 0, 2.0f);
  EXPECT_TRUE(lp.is_feasible(DVector({0, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({1, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({3.5, 0})));
  EXPECT_TRUE(!lp.is_feasible(DVector({-1.5, 0})));
  EXPECT_TRUE(!lp.is_feasible(DVector({5, 0})));
  EXPECT_TRUE(!lp.is_feasible(DVector({-1.0f, 0})));
}

TEST(LPTest, IsFeasibleForFreeVarAndRangeRow) {
  LP lp;
  lp.add_column(ColType::Free, -inf, inf);
  lp.add_row(RowType::Range, -5, 5);
  lp.add_logicals();
  lp.add_value(0, 0, 1.0);
  EXPECT_TRUE(lp.is_feasible(DVector({0, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({5, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({-5, 0})));
  EXPECT_TRUE(!lp.is_feasible(DVector({6, 0})));
  EXPECT_TRUE(!lp.is_feasible(DVector({-6, 0})));
}

TEST(LPTest, IsFeasibleForFreeVarAndNonBindingRow) {
  LP lp;
  lp.add_column(ColType::Free, -inf, inf);
  lp.add_row(RowType::NonBinding, -inf, inf);
  lp.add_logicals();
  lp.add_value(0, 0, 1.0);
  EXPECT_TRUE(lp.is_feasible(DVector({0, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({5, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({-5, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({inf, 0})));
  EXPECT_TRUE(lp.is_feasible(DVector({-inf, 0})));
}
