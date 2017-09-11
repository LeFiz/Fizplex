#include "svector.h"
#include "test.h"

Test(SVector, length, "empty") {
  SVector v;
  EXPECT(v.length() == 0);
}

Test(SVector, length, "non-empty") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  EXPECT(v.length() == 4);
}

Test(SVector, add_value, "valid entry") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  const double d = 3.14;
  v.add_value(3, d);
  EXPECT(v.length() == 5);
  EXPECT(is_eq(v.get_value(3), d));
}

Test(SVector, add_value, "double entry") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  const double d = 3;
  v.add_value(1, d);
  EXPECT(v.length() == 5);
}

Test(SVector, equality, "equal but different order") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  SVector w = {{2, 9}, {0, -2}, {7, -2.111111111111}, {1, 3}};
  EXPECT(v == w);
  EXPECT(w == v);
}

Test(SVector, equality, "inequal, one empty") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  SVector w;
  EXPECT(!(v == w));
  EXPECT(!(w == v));
}

Test(SVector, inequality, "equal but different order") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  SVector w = {{2, 9}, {0, -2}, {7, -2.111111111111}, {1, 3}};
  EXPECT(!(v != w));
  EXPECT(!(w != v));
}

Test(SVector, inequality, "inequal, one empty") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  SVector w;
  EXPECT(v != w);
  EXPECT(w != v);
}

Test(SVector, iterator, "nonempty, change values") {
  SVector v = {{0, -2}, {1, 3}, {2, 9}, {7, -2.111111111111}};
  for (auto &n : v) {
    n.value *= 3;
  }
  EXPECT(is_eq(v.get_value(2), 27));
}

Test(SVector, iterator, "empty") {
  SVector v;
  for (auto &n : v) {
    n.value *= 3;
    EXPECT(false);
  }
}

Test(SVector, scalar_mult, "non-empty") {
  SVector v = {{0, -2}, {1, 3}, {4, 9}};
  SVector w = {{0, -4}, {1, 6}, {4, 18}};
  EXPECT(2.0f * v == w);
}
