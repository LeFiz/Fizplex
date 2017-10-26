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

TEST(VerificationTest, Blend) {
  MPSReader r("./test/blend.mps");
  LP lp = r.read_lp();

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-3.0812149846E+01, spx.get_z()));
}

TEST(ExperimentalVerificationTest, Brandy) {
  MPSReader r("./test/brandy.mps");
  LP lp = r.read_lp();

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(1.5185098965E+03, spx.get_z()));
  EXPECT_DOUBLE_EQ(1.5185098965E+03, spx.get_z());
}

TEST(ExperimentalVerificationTest, Agg) {
  MPSReader r("./test/agg.mps");
  LP lp = r.read_lp();

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-3.5991767287E+07, spx.get_z()));
  EXPECT_DOUBLE_EQ(-3.5991767287E+07, spx.get_z());
}

TEST(ExperimentalVerificationTest, Stocfor1) {
  MPSReader r("./test/stocfor1.mps");
  LP lp = r.read_lp();

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-4.1131976219E+04, spx.get_z()));
  EXPECT_DOUBLE_EQ(-4.1131976219E+04, spx.get_z());
}
