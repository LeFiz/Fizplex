
#include "base.h"
#include "test.h"

Test(Base, invert, "invertible, no re-ordering") {
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
  EXPECT(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);
  EXPECT(e0 == r0);
  EXPECT(e1 == r1);
  EXPECT(e2 == r2);
}

Test(Base, invert, "invertible, with re-ordering") {
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
  EXPECT(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);
  EXPECT(e0 == r0);
  EXPECT(e1 == r1);
  EXPECT(e2 == r2);
}

Test(Base, invert, "singular") {
  ColMatrix m(3, 3,
              {{{0, 1}, {1, 1}, {2, 1}},
               {{0, 1}, {1, 1}, {2, 1}},
               {{0, 1}, {1, 1}, {2, 1}}});
  Base b(m);
  EXPECT(b.invert() == false);
}

Test(Base, updateVecWithETM, "DVector, with reordering") {
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
  r0[0] = -2.0f / 71;
  r0[1] = 17.0f / 71;
  r0[2] = 37.0f / 71;
  r1[0] = 8.0f / 71;
  r1[1] = 3.0f / 71;
  r1[2] = -6.0f / 71;
  r2[0] = -5.0f / 71;
  r2[1] = 7.0f / 71;
  r2[2] = -14.0f / 71;

  EXPECT(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);

  EXPECT(e0 == r0);
  EXPECT(e1 == r1);
  EXPECT(e2 == r2);
}

Test(Base, updateVecWithETM, "DVector, no reordering") {
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

  EXPECT(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);

  EXPECT(e0 == r0);
  EXPECT(e1 == r1);
  EXPECT(e2 == r2);
}
