
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

Test(LP, add_logicals, "all") {
  LP lp;
  lp.add_column(ColType::Bounded, 0, 5);
  lp.add_row(RowType::Equality, 15, 15);
  lp.add_logicals();
  EXPECT(lp.column_count() == 2);
  EXPECT(is_eq(lp.get_value(0, 1), 1));
}
