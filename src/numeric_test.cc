
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
