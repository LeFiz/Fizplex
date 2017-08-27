
#include "colmatrix.h"
#include "test.h"

Test(ColMatrix, row_count, "empty matrix") {
  ColMatrix m;
  EXPECT(m.row_count() == 0);
}

Test(ColMatrix, row_count, "non-empty matrix") {
  ColMatrix m(3, 4);
  EXPECT(m.row_count() == 3);
}

Test(ColMatrix, col_count, "empty matrix") {
  ColMatrix m;
  EXPECT(m.col_count() == 0);
}

Test(ColMatrix, col_count, "non-empty matrix") {
  ColMatrix m(3, 4);
  EXPECT(m.col_count() == 4);
}

Test(ColMatrix, equality_operator, "equal matrices") {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {2, 4}, {1, 3}},
               {{0, 1}, {1, -1}, {2, -2}}});
  ColMatrix mm(3, 3,
               {{{0, 7}, {2, -3}, {1, 0.0}},
                {{0, 2}, {1, 3}, {2, 4}},
                {{0, 1}, {1, -1}, {2, -2}}});
  EXPECT(m == mm);
  EXPECT(mm == m);
  EXPECT(!(m != mm));
  EXPECT(!(mm != m));
}

Test(ColMatrix, equality_operator, "inequal matrices, wrong dimensions") {
  ColMatrix m(4, 5,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {2, 4}, {1, 3}},
               {{0, 1}, {1, -1}, {2, -2}}});
  ColMatrix mm(3, 3,
               {{{0, 7}, {2, -3}, {1, 0.0}},
                {{0, 2}, {1, 3}, {2, 4}},
                {{0, 1}, {1, -1}, {2, -2}}});
  EXPECT(!(m == mm));
  EXPECT(!(mm == m));
  EXPECT(m != mm);
  EXPECT(mm != m);
}

Test(ColMatrix, assign_operator, "non-empty matrix") {
  ColMatrix m(1, 1);
  ColMatrix mm(3, 3,
               {{{0, 7}, {2, -3}, {1, 0.0}},
                {{0, 2}, {1, 3}, {2, 4}},
                {{0, 1}, {1, -1}, {2, -2}}});
  m = mm;
  EXPECT(m == mm);
}

Test(ColMatrix, column, "non-empty matrix") {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  SVector v = {{0, 2}, {1, 3}, {2, 4}};
  EXPECT(m.column(1) == v);
}

Test(ColMatrix, swap_column, "non-empty matrix") {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  SVector v0 = m.column(0);
  SVector v2 = m.column(2);
  m.swap_columns(0, 2);
  EXPECT(m.column(0) == v2);
  EXPECT(m.column(2) == v0);
}
