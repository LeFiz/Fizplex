cmake_minimum_required(VERSION 3.2)
project(fizplex)

### Include Googletest ###
find_package (Threads)

set(GTEST_DIR lib/gtest/googletest)
include_directories(SYSTEM
  ${GTEST_DIR}
  ${GTEST_DIR}/include
  )
add_library(gtest ${GTEST_DIR}/src/gtest-all.cc ${GTEST_DIR}/src/gtest_main.cc)

### General warnings and flags ###
add_compile_options(
  -Wall -Wextra -Wfloat-equal -Wcast-qual -Wcast-align -Wconversion -Wshadow
  -Wswitch-default -Wmissing-declarations -Wmissing-include-dirs -Wuninitialized
  -Wwrite-strings -Wold-style-cast -Woverloaded-virtual -Wsign-promo -pedantic -std=c++17 -O2
  )

add_library(core
  src/svector.cc
  src/dvector.cc
  src/colmatrix.cc
  src/base.cc
  src/lp.cc
  src/simplex.cc
  src/mpsreader.cc
  src/pricer.cc
  src/ratio_tester.cc
  src/node_on_integer.cc
  src/simplex_on_integers.cc
  )

add_executable(tests
  src/main_test.cc
  src/svector_test.cc
  src/dvector_test.cc
  src/colmatrix_test.cc
  src/base_test.cc
  src/lp_test.cc
  src/simplex_test.cc
  src/simplex_on_int_test.cc
  src/mpsreader_test.cc
  src/pricer_test.cc
  src/ratio_tester_test.cc
  src/debug_test.cc
  src/numeric_test.cc
  src/verification_test.cc
  )
target_link_libraries(tests core gtest ${CMAKE_THREAD_LIBS_INIT})

add_executable(fizplex src/main_release.cc)
target_link_libraries(fizplex core)
