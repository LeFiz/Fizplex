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
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(filter) = "-Verific*";
  auto errors = RUN_ALL_TESTS();
  if (errors == 0) {
    ::testing::GTEST_FLAG(filter) = "Verific*";
    errors += RUN_ALL_TESTS();
  }
  return errors;
}
