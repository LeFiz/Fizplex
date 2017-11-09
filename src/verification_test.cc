#include "mpsreader.h"
#include "simplex.h"
#include "gtest/gtest.h"

TEST(VerificationTest, Afiro) {
  LP lp = MPSReader::read_lp("./test/afiro.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-4.6475314286E02, spx.get_z()));
}

TEST(VerificationTest, Sc105) {
  LP lp = MPSReader::read_lp("./test/sc105.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-5.2202061212E+01, spx.get_z()));
}

TEST(VerificationTest, Blend) {
  LP lp = MPSReader::read_lp("./test/blend.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-3.0812149846E+01, spx.get_z()));
}

TEST(VerificationTest, Stocfor1) {
  LP lp = MPSReader::read_lp("./test/stocfor1.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-4.1131976219E+04, spx.get_z()));
}

TEST(ExperimentalVerificationTest, Brandy) {
  LP lp = MPSReader::read_lp("./test/brandy.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(1.5185098965E+03, spx.get_z()));
  EXPECT_DOUBLE_EQ(1.5185098965E+03, spx.get_z());
}

TEST(ExperimentalVerificationTest, Agg) {
  LP lp = MPSReader::read_lp("./test/agg.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq(-3.5991767287E+07, spx.get_z()));
  EXPECT_DOUBLE_EQ(-3.5991767287E+07, spx.get_z());
}
