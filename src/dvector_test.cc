#include "dvector.h"
#include "numeric.h"
#include "test.h"

Test(DVector, DVector, "with dimension") {
  DVector v(5);
  EXPECT(v.dimension() == 5);
}

Test(DVector, resize, "add to empty vector") {
  DVector v;
  v.resize(1);
  EXPECT(v.dimension() == 1);
}

Test(DVector, resize, "add to non-empty vector") {
  DVector v(5);
  v.resize(10);
  EXPECT(v.dimension() == 10);
}

Test(DVector, resize, "values remain when adding to a non-empty vector") {
  DVector v(5);
  const double d = 3.14;
  v[4] = d;
  v.resize(10);
  EXPECT(is_eq(v[4], d));
}

Test(DVector, resize, "can shrink vectors") {
  DVector v(5);
  v.resize(1);
  EXPECT(v.dimension() == 1);
}

Test(DVector, resize, "values remain when shrinking a vector") {
  DVector v(5);
  const double d = 3.14;
  v[0] = d;
  v.resize(1);
  EXPECT(is_eq(v[0], d));
}

Test(DVector, append, "adds the parameter") {
  DVector v(5);
  const double d = 1.23;
  v.append(d);
  EXPECT(is_eq(v[5], d));
}

Test(DVector, member_access, "existing element") {
  DVector v(5);
  double d = 3.11111;
  v[2] = d;
  EXPECT(is_eq(v[2], d));
}

Test(DVector, dot_product, "zero vector") {
  DVector v(10), w(10);
  EXPECT(is_zero(v * w));
}

Test(DVector, dot_product, "non-zero vector") {
  DVector v(10), w(10);
  const double d = 7.123456;
  v[3] = 1;
  w[3] = d;
  v[7] = 0.5;
  w[7] = 2.0;
  EXPECT(is_eq(v * w, d + 1));
  EXPECT(is_eq(v * w, w * v));
}

Test(DVector, dot_product_svector, "zero vector") {
  DVector v(10);
  SVector w;
  EXPECT(is_zero(v * w));
}

Test(DVector, dot_product_svector, "non-zero vector") {
  DVector v(10);
  SVector w;
  const double d = 7.123456;
  v[3] = 1;
  w.add_value(3, d);
  v[7] = 0.5f;
  w.add_value(7, 2.0);
  EXPECT(is_eq(v * w, d + 1));
}

Test(DVector, equality, "empty vectors") {
  DVector v, w;
  EXPECT(v == w);
}

Test(DVector, equality, "non-empty, equal") {
  DVector v(3), w(3);
  const double d = 2.123456;
  v[0] = 1;
  w[0] = 1;
  v[2] = d;
  w[2] = d;
  EXPECT(v == w);
  EXPECT(!(v != w));
}

Test(DVector, equality, "non-empty, inequal") {
  DVector v(3), w(3);
  const double d = 2.123456;
  v[0] = 1;
  w[0] = 1;
  v[1] = 17;
  v[2] = d;
  w[2] = d;
  EXPECT(!(v == w));
  EXPECT((v != w));
}

Test(DVector, mult_with_scalar, "non-empty") {
  DVector v(3), w(3);
  v[0] = 1;
  w[0] = 2;
  v[1] = 17.5;
  w[1] = 35.0;
  v[2] = 0;
  w[2] = 0;
  EXPECT(2.0f * v == w);
}

Test(DVector, operator_compound_subtraction, "DVector") {
  DVector v(3), w(3);
  v[0] = 1;
  w[0] = 2;
  v[1] = 17.5;
  w[1] = 35.0;
  v[2] = 0;
  w[2] = 0;
  w -= v;
  EXPECT(v == w);
}

Test(DVector, operator_compound_subtraction, "SVector") {
  DVector v(3), w(3);
  v[0] = 4;
  v[1] = 40;
  v[2] = 0.5;
  w[0] = 2;
  w[1] = 35.0;
  w[2] = 0.75;

  SVector sv = {{0, 2}, {1, 5}, {2, -0.25}};
  v -= sv;
  EXPECT(v == w);
}
