#include <array>
#include <iostream>
#include <random>

#include "base.h"
#include "colmatrix.h"
#include "lp.h"
#include "simplex.h"
#include "svector.h"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::GTEST_FLAG(filter) = "*Israel*";
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
