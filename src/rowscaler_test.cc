#include "rowscaler.h"
#include "gtest/gtest.h"

class RowScalerTest : public ::testing::Test {
public:
  RowScalerTest() : lp(), scaler(lp) {
    lp.add_column(ColType::Free, -inf, inf);
    lp.add_column(ColType::Free, -inf, inf);
    lp.add_column(ColType::Free, -inf, inf);
  }

  LP lp;
  RowScaler scaler;
};

TEST_F(RowScalerTest, ZeroRowIsUnchanged) {
  lp.add_row(RowType::NonBinding, -inf, inf);

  scaler.scale_all();
  EXPECT_DOUBLE_EQ(0.0, lp.get_value(0, 0));
}

TEST_F(RowScalerTest, LargerThanOneScalingFactor) {
  lp.add_row(RowType::Range, -3, 9, {-0.6, 0.0, 3.0});

  scaler.scale_all();
  EXPECT_DOUBLE_EQ(-0.2, lp.get_value(0, 0));
  EXPECT_DOUBLE_EQ(1.0, lp.get_value(0, 2));
  EXPECT_DOUBLE_EQ(3.0, lp.b[0]);
}

TEST_F(RowScalerTest, MultipleRows) {
  lp.add_row(RowType::Range, 0.0, 30, {-0.9, 0.0, 3.0});
  lp.add_row(RowType::Range, 0.0, 9, {-0.9, 0.0, 0.18});

  scaler.scale_all();
  EXPECT_DOUBLE_EQ(-0.3, lp.get_value(0, 0));
  EXPECT_DOUBLE_EQ(10.0, lp.b[0]);

  EXPECT_DOUBLE_EQ(-1.0, lp.get_value(1, 0));
  EXPECT_DOUBLE_EQ(0.2, lp.get_value(1, 2));
  EXPECT_DOUBLE_EQ(10.0, lp.b[1]);
}
