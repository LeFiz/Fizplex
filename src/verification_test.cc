#include "mpsreader.h"
#include "simplex.h"
#include "gtest/gtest.h"

namespace fizplex {

TEST(VerificationTest, Afiro) {
  LP lp = MPSReader::read_lp("./test/afiro.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(-4.6475314286E02, spx.get_z()));
}

TEST(VerificationTest, Sc105) {
  LP lp = MPSReader::read_lp("./test/sc105.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(-5.2202061212E+01, spx.get_z()));
}

TEST(VerificationTest, Blend) {
  LP lp = MPSReader::read_lp("./test/blend.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(-3.0812149846E+01, spx.get_z()));
}

TEST(VerificationTest, Stocfor1) {
  LP lp = MPSReader::read_lp("./test/stocfor1.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(-4.1131976219E+04, spx.get_z()));
}

TEST(VerificationTest, Brandy) {
  LP lp = MPSReader::read_lp("./test/brandy.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(1.5185098965E+03, spx.get_z()));
}

TEST(VerificationTest, Agg) {
  LP lp = MPSReader::read_lp("./test/agg.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(-3.5991767287E+07, spx.get_z()));
}

TEST(VerificationTest, Israel) {
  LP lp = MPSReader::read_lp("./test/israel.mps");

  Simplex spx(lp);
  spx.solve();
  EXPECT_TRUE(is_eq_norm(-8.9664482186E+05, spx.get_z()));
}

} // namespace fizplex
