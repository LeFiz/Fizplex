#include "svector.h"
#include "test.h"

Test(SVector, all, "works") {
  SVector v;
  EXPECT(v.length() == 0);
  SVector vv = {{3, 0.000f}, {19, 0}};
  EXPECT(v == vv);
  v.add_value(3, -0.12345);
  EXPECT(v.length() == 1);
  EXPECT(v != vv);
  EXPECT(vv != v);

  SVector w = {{0, -2.0}, {1, 3}, {2, 9}};
  SVector ww = {{0, -2}, {1, 3}, {2, 9}, {7, 0}};
  EXPECT(w.length() == 3);
  EXPECT(ww.length() == 4);
  EXPECT(w == ww);
  EXPECT(ww == w);
}
