#include "dvector.h"
#include "numeric.h"
#include "test.h"

Test(DVector, DVector, "with dimension") {
  DVector v(5);
  EXPECT(v.dimension() == 5);
}

Test(DVector, member_access, "existing element") {
  DVector v(5);
  double d = 3.11111f;
  v[2] = d;
  EXPECT(is_eq(v[2], d));
}

Test(DVector, dot_product, "zero vector") {
  DVector v(10), w(10);
  EXPECT(is_zero(v * w));
}

Test(DVector, dot_product, "non-zero vector") {
  DVector v(10), w(10);
  const double d = 7.123456f;
  v[3] = 1;
  w[3] = d;
  v[7] = 0.5f;
  w[7] = 2.0f;
  EXPECT(is_eq(v * w, d + 1));
  EXPECT(is_eq(v * w, w * v));
}
