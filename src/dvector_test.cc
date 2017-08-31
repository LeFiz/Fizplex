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
