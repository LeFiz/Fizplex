#include <iostream>

#include "base.h"
#include "test.h"

Test(Base, invert, "invertible, no re-ordering") {
  ColMatrix<3> m = {
      {{0, 7}, {2, -3}}, {{0, 2}, {1, 3}, {2, 4}}, {{0, 1}, {1, -1}, {2, -2}}};
  Base<3> b(m);
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
  ColMatrix<3> m = {
      {{1, 7}, {2, -3}}, {{0, 2}, {1, 3}, {2, 4}}, {{0, 1}, {1, -1}, {2, -2}}};
  Base<3> b(m);
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
  Base<3> b;
  EXPECT(b.invert() == false);

  std::cout << "in singular\n";
  ColMatrix<3> m = {{{0, 1}, {1, 1}, {2, 1}},
                    {{0, 1}, {1, 1}, {2, 1}},
                    {{0, 1}, {1, 1}, {2, 1}}};
  b.setBase(m);
  EXPECT(b.invert() == false);
}
