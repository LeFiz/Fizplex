
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
