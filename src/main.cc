#include <array>
#include <iostream>
#include <random>

#include "base.h"
#include "colmatrix.h"
#include "lp.h"
#include "simplex.h"
#include "svector.h"
#include "gtest/gtest.h"

#include "test.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  TestRunner::run();

  return RUN_ALL_TESTS();
}
