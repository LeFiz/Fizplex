
#include "numeric.h"
#include "test.h"

Test(Numeric, is_eq, "equal numbers") { EXPECT(is_eq(2.00000f, 2)); }

Test(Numeric, is_eq, "inequal numbers") { EXPECT(!is_eq(2.00000f, 3)); }

Test(Numeric, is_finite, "finite values") {
  EXPECT(is_finite(2.00000f));
  EXPECT(is_finite(31));
  EXPECT(is_finite(-31.000000001f));
}

Test(Numeric, is_finite, "infinite values") {
  EXPECT(!is_finite(inf));
  EXPECT(!is_finite(-inf));
}

Test(Numeric, is_infinite, "infinite values") {
  EXPECT(is_infinite(inf));
  EXPECT(is_infinite(-inf));
}

Test(Numeric, is_infinite, "finite values") {
  EXPECT(!is_infinite(2.00000f));
  EXPECT(!is_infinite(31));
  EXPECT(!is_infinite(-31.000000001f));
}

Test(Numeric, is_ge, "finite values") {
  EXPECT(is_ge(1.0f, 0.0f));
  EXPECT(is_ge(-2.111f, -999));
}
