#include "mpsreader.h"
#include "simplex.h"
#include "gtest/gtest.h"

TEST(VerificationTest, Afiro) {
  MPSReader r("./test/afiro.mps");
  LP lp = r.read_lp();

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-4.6475314286E02, spx.get_z()));
}

TEST(VerificationTest, Sc105) {
  MPSReader r("./test/sc105.mps");
  LP lp = r.read_lp();

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-5.2202061212E+01, spx.get_z()));
}
