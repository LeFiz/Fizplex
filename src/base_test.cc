#include "base.h"
#include "gtest/gtest.h"

TEST(BaseTest, InvertRegularNoReordering) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  Base b(m);
  SVector e0 = {{0, 1}};
  SVector e1 = {{1, 1}};
  SVector e2 = {{2, 1}};
  const SVector r0 = {{0, -2}, {1, 3}, {2, 9}};
  const SVector r1 = {{0, 8}, {1, -11}, {2, -34}};
  const SVector r2 = {{0, -5}, {1, 7}, {2, 21}};
  EXPECT_TRUE(b.invert());
  b.ftran(e0);
  b.ftran(e1);
  b.ftran(e2);
  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

TEST(BaseTest, InvertRegularWithReordering) {
  ColMatrix m(3, 3,
              {{{1, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  Base b(m);
  SVector e0 = {{0, 1}};
  SVector e1 = {{1, 1}};
  SVector e2 = {{2, 1}};
  const SVector r0 = {{0, -2.0 / 71}, {1, 17.0 / 71}, {2, 37.0 / 71}};
  const SVector r1 = {{0, 8.0 / 71}, {1, 3.0 / 71}, {2, -6.0 / 71}};
  const SVector r2 = {{0, -5.0 / 71}, {1, 7.0 / 71}, {2, -14.0 / 71}};
  EXPECT_TRUE(b.invert());
  b.ftran(e0);
  b.ftran(e1);
  b.ftran(e2);
  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

TEST(BaseTest, InvertSingular) {
  ColMatrix m(3, 3,
              {{{0, 1}, {1, 1}, {2, 1}},
               {{0, 1}, {1, 1}, {2, 1}},
               {{0, 1}, {1, 1}, {2, 1}}});
  Base b(m);
  EXPECT_TRUE(b.invert() == false);
}

TEST(BaseTest, FTranDVectorWithReordering) {
  ColMatrix m(3, 3,
              {{{1, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  Base b(m);
  DVector e0(3), e1(3), e2(3);
  e0[0] = 1;
  e1[1] = 1;
  e2[2] = 1;

  DVector r0(3), r1(3), r2(3);
  r0[0] = -2.0 / 71;
  r0[1] = 17.0 / 71;
  r0[2] = 37.0 / 71;
  r1[0] = 8.0 / 71;
  r1[1] = 3.0 / 71;
  r1[2] = -6.0 / 71;
  r2[0] = -5.0 / 71;
  r2[1] = 7.0 / 71;
  r2[2] = -14.0 / 71;

  EXPECT_TRUE(b.invert());
  b.ftran(e0);
  b.ftran(e1);
  b.ftran(e2);

  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

TEST(BaseTest, FTranDVectorNoReordering) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  Base b(m);
  DVector e0(3), e1(3), e2(3);
  e0[0] = 1;
  e1[1] = 1;
  e2[2] = 1;

  DVector r0(3), r1(3), r2(3);
  r0[0] = -2;
  r0[1] = 3;
  r0[2] = 9;
  r1[0] = 8;
  r1[1] = -11;
  r1[2] = -34;
  r2[0] = -5;
  r2[1] = 7;
  r2[2] = 21;

  EXPECT_TRUE(b.invert());
  b.ftran(e0);
  b.ftran(e1);
  b.ftran(e2);

  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

TEST(BaseTest, BTranDVectorNoReordering) {
  ColMatrix m(3, 3,
              {{{0, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  Base b(m);
  DVector e0(3), e1(3), e2(3);
  e0[0] = 1;
  e1[1] = 1;
  e2[2] = 1;

  DVector r0(3), r1(3), r2(3);
  r0[0] = -2;
  r0[1] = 8;
  r0[2] = -5;
  r1[0] = 3;
  r1[1] = -11;
  r1[2] = 7;
  r2[0] = 9;
  r2[1] = -34;
  r2[2] = 21;

  EXPECT_TRUE(b.invert());
  b.btran(e0);
  b.btran(e1);
  b.btran(e2);

  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

TEST(BaseTest, BTranDVectorWithReordering) {
  ColMatrix m(3, 3,
              {{{1, 7}, {2, -3}},
               {{0, 2}, {1, 3}, {2, 4}},
               {{0, 1}, {1, -1}, {2, -2}}});
  Base b(m);
  DVector e0(3), e1(3), e2(3);
  e0[0] = 1;
  e1[1] = 1;
  e2[2] = 1;

  DVector r0(3), r1(3), r2(3);
  r0[0] = -2.0 / 71;
  r0[1] = 8.0 / 71;
  r0[2] = -5.0 / 71;
  r1[0] = 17.0 / 71;
  r1[1] = 3.0 / 71;
  r1[2] = 7.0 / 71;
  r2[0] = 37.0 / 71;
  r2[1] = -6.0 / 71;
  r2[2] = -14.0 / 71;

  EXPECT_TRUE(b.invert());
  b.btran(e0);
  b.btran(e1);
  b.btran(e2);

  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}
