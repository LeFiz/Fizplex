#include "lp.h"
#include "gtest/gtest.h"

// TODO structure by case: LPTestRangeRow - b is set properly, logical is set

class LPTestFeasibility : public ::testing::Test {
public:
  LP lp;
  void check_feasible(bool feasible, const std::vector<DVector> &vecs) {
    for (auto &v : vecs)
      EXPECT_TRUE(lp.is_feasible(v) == feasible);
  }
};

TEST_F(LPTestFeasibility, IsFeasibleForBoundedVarAndRangeRow) {
  lp.add_column(ColType::Bounded, -1.0, 5);
  lp.add_row(RowType::Range, -3, 7);
  lp.add_value(0, 0, 2.0);

  check_feasible(true, {{0}, {-1}, {3.5}});
  check_feasible(false, {{-1.5}, {4}});
}

TEST_F(LPTestFeasibility, IsFeasibleForFreeVarAndRangeRow) {
  lp.add_column(ColType::Free, -inf, inf);
  lp.add_row(RowType::Range, -5, 5);
  lp.add_value(0, 0, 1.0);

  check_feasible(true, {{0}, {-5}, {5}});
  check_feasible(false, {{-6}, {6}});
}

TEST_F(LPTestFeasibility, IsFeasibleForFreeVarAndNonBindingRow) {
  lp.add_column(ColType::Free, -inf, inf);
  lp.add_row(RowType::NonBinding, -inf, inf);
  lp.add_value(0, 0, 1.0);

  check_feasible(true, {{0}, {-5}, {5}, {-inf}, {inf}});
}

TEST(LPTestEmptyLP, ColumnAndRowCount) {
  LP lp;
  EXPECT_TRUE(lp.column_count() == 0);
  EXPECT_TRUE(lp.row_count() == 0);
}

TEST(LPTest, AddColumnIncreasesColumnCount) {
  LP lp;
  lp.add_column(ColType::Bounded, -1.33333, 302);
  EXPECT_TRUE(lp.column_count() == 1);
}

TEST(LPTest, AddAndEditColumn) {
  LP lp;
  lp.add_column(ColType::Bounded, -1.0, 2.0);
  lp.column_header(0u).upper = 3.14;
  EXPECT_DOUBLE_EQ(3.14, lp.column_header(0u).upper);
}

TEST(LPTest, AddRowFromValues) {
  LP lp;
  lp.add_row(RowType::Range, -1.33333, 302);
  EXPECT_TRUE(lp.row_count() == 1);
  EXPECT_TRUE(lp.row_header(0u).type == RowType::Range);
}

TEST(LPTest, AddAndEditRow) {
  LP lp;
  lp.add_row(RowType::Range, -1.0, 2.0);
  lp.update_row_header(0u, RowType::Range, 0.0, 3.0);
  EXPECT_DOUBLE_EQ(0.0, lp.row_header(0u).lower);
}

TEST(LPTest, AddRowFromRow) {
  LP lp;
  LP::Row r(RowType::LE, -1.33333, 302);
  lp.add_row(r);
  EXPECT_TRUE(lp.row_count() == 1);
  EXPECT_TRUE(lp.row_header(0u).type == RowType::LE);
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
