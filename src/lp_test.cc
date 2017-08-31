
#include "lp.h"
#include "test.h"

Test(LP, column_count, "empty LP") {
  LP lp;
  EXPECT(lp.column_count() == 0);
}

Test(LP, row_count, "empty LP") {
  LP lp;
  EXPECT(lp.row_count() == 0);
}

Test(LP, add_column, "empty LP") {
  LP lp;
  lp.add_column(ColType::Bounded, -1.33333, 302);
  EXPECT(lp.column_count() == 1);
}

Test(LP, add_row, "empty LP") {
  LP lp;
  lp.add_row(RowType::Range, -1.33333, 302);
  EXPECT(lp.row_count() == 1);
}

Test(LP, get_value, "single value in LP") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  const double d = 3.14;
  lp.add_value(0, 0, d);
  EXPECT(is_eq(lp.get_value(0, 0), d));
}

Test(LP, get_value, "non-existing value") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  EXPECT(is_zero(lp.get_value(0, 0)));
}

Test(LP, set_b, "range row") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  lp.set_b();
  EXPECT(is_eq(lp.b[0], 15));
}

Test(LP, set_b, "GE row") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::GE, 3, 15);
  lp.set_b();
  EXPECT(is_eq(lp.b[0], 3));
}

Test(LP, add_obj_value, "non-empty LP") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Range, 3, 15);
  const double d = 3.14;
  lp.add_logicals();
  lp.add_obj_value(0, d);
  EXPECT(is_eq(lp.get_obj_value(0), d));
}

Test(LP, add_logicals, "all types") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Equality, 1, 1);
  lp.add_row(RowType::Range, 3, 15);
  lp.add_row(RowType::LE, -inf, 7);
  lp.add_row(RowType::NonBinding, -inf, inf);
  lp.add_row(RowType::GE, 3, inf);
  lp.add_logicals();

  EXPECT(lp.column_count() == 6);

  EXPECT(lp.column_header(1).type == ColType::Fixed);
  EXPECT(lp.column_header(2).type == ColType::Bounded);
  EXPECT(lp.column_header(3).type == ColType::LowerBound);
  EXPECT(lp.column_header(4).type == ColType::Free);
  EXPECT(lp.column_header(5).type == ColType::UpperBound);

  EXPECT(is_eq(lp.column_header(1).lower, 0));
  EXPECT(is_eq(lp.column_header(1).upper, 0));
  EXPECT(is_eq(lp.column_header(2).lower, 0));
  EXPECT(is_eq(lp.column_header(2).upper, 12));
  EXPECT(is_eq(lp.column_header(3).lower, 0));
  EXPECT(is_eq(lp.column_header(5).upper, 0));

  for (int i = 0; i < 5; i++)
    EXPECT(is_eq(lp.get_value(i, 1 + i), 1));
}
