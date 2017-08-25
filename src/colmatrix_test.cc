
#include "colmatrix.h"
#include "test.h"

Test(ColMatrix, all, "works") {
  ColMatrix<3> m = {
      {{0, 7}, {2, -3}}, {{0, 2}, {1, 3}, {2, 4}}, {{0, 1}, {1, -1}, {2, -2}}};
  ColMatrix<3> mm = {{{0, 7}, {2, -3}, {1, 0.0}},
                     {{0, 2}, {1, 3}, {2, 4}},
                     {{0, 1}, {1, -1}, {2, -2}}};
  EXPECT(m == mm);
  EXPECT(mm == m);
  SVector v = {{0, 2}, {1, 3}, {2, 4}};
  EXPECT(m.column(1) == v);
}
