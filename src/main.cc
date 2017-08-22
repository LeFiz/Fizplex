#include <random>
#include <iostream>
#include <chrono>

#include "svector.h"
#include "colmatrix.h"
#include "base.h"

#define EXPECT_TRUE(statement) (statement ? std::cerr : (std::cerr << "Error! Expected true got false! In " << __FUNCTION__ << " (Line " << __LINE__ << " in " << __FILE__ << ")\n"))

#define EXPECT_FALSE(statement) (!(statement) ? std::cerr : (std::cerr << "Error! Expected false got true! In " << __FUNCTION__ << " (Line " << __LINE__ << " in " << __FILE__ << ")\n"))


void test_svector();
void test_colmatrix();
void test_base_no_reordering();
void test_base_with_reordering();
void test_base_singular_matrix();

using TestFunction = void (*) ();
using Timer = std::chrono::high_resolution_clock;

int main() {
  std::vector<TestFunction> tests;
  tests.push_back(test_svector);
  tests.push_back(test_colmatrix);
  tests.push_back(test_base_no_reordering);
  tests.push_back(test_base_with_reordering);
  tests.push_back(test_base_singular_matrix);
  for(auto& t : tests) {
    auto start = Timer::now();
    t();
    auto end = Timer::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << duration.count() * 1000 << "ms" << std::endl;
  }
  return 0;
}

void test_svector() {
  SVector v;
  EXPECT_TRUE(v.length() == 0);
  SVector vv = {{3,0.000f}, {19, 0}};
  EXPECT_TRUE(v == vv);
  v.add_value(3,-0.12345);
  EXPECT_TRUE(v.length() == 1);
  EXPECT_TRUE(v != vv);
  EXPECT_TRUE(vv != v);

  SVector w = {{0,-2.0}, {1,3}, {2,9}};
  SVector ww = {{0,-2}, {1,3}, {2,9}, {7,0}};
  EXPECT_TRUE(w.length() == 3);
  EXPECT_TRUE(ww.length() == 4);
  EXPECT_TRUE(w==ww);
  EXPECT_TRUE(ww==w);
}

void test_colmatrix() {
  ColMatrix<3> m = {{{0,7}, {2,-3}},
                    {{0,2}, {1,3}, {2,4}},
                    {{0,1}, {1,-1}, {2,-2}}};
  ColMatrix<3> mm = {{{0,7}, {2,-3}, {1,0.0}},
                    {{0,2}, {1,3}, {2,4}},
                    {{0,1}, {1,-1}, {2,-2}}};
  EXPECT_TRUE(m==mm);
  EXPECT_TRUE(mm==m);
  SVector v = {{0,2}, {1,3}, {2,4}};
  EXPECT_TRUE(m.column(1) == v);
}

void test_base_no_reordering() {
  ColMatrix<3> m = {{{0,7}, {2,-3}},
                    {{0,2}, {1,3}, {2,4}},
                    {{0,1}, {1,-1}, {2,-2}}};
  Base<3> b(m);
  SVector e0 = {{0, 1}};
  SVector e1 = {{1, 1}};
  SVector e2 = {{2, 1}};
  const SVector r0 = {{0,-2}, {1,3}, {2,9}};
  const SVector r1 = {{0,8}, {1,-11}, {2,-34}};
  const SVector r2 = {{0,-5}, {1,7}, {2,21}};
  EXPECT_TRUE(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);
  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

void test_base_with_reordering() {
  ColMatrix<3> m = {{{1,7}, {2,-3}},
                    {{0,2}, {1,3}, {2,4}},
                    {{0,1}, {1,-1}, {2,-2}}};
  Base<3> b(m);
  SVector e0 = {{0, 1}};
  SVector e1 = {{1, 1}};
  SVector e2 = {{2, 1}};
  const SVector r0 = {{0,-2.0/71}, {1,17.0/71}, {2,37.0/71}};
  const SVector r1 = {{0,8.0/71}, {1,3.0/71}, {2,-6.0/71}};
  const SVector r2 = {{0,-5.0/71}, {1,7.0/71}, {2,-14.0/71}};
  EXPECT_TRUE(b.invert());
  b.updateVec(e0);
  b.updateVec(e1);
  b.updateVec(e2);
  EXPECT_TRUE(e0 == r0);
  EXPECT_TRUE(e1 == r1);
  EXPECT_TRUE(e2 == r2);
}

void test_base_singular_matrix()
{
  Base<3> b;
  EXPECT_FALSE(b.invert()); // Empty base / matrix

  ColMatrix<3> m = {{{0,1}, {1,1}, {2,1}},
                    {{0,1}, {1,1}, {2,1}},
                    {{0,1}, {1,1}, {2,1}}};
  b.setBase(m);
  EXPECT_FALSE(b.invert());
}

//  const int m = 1000;
//  const int dense = m / 5;
//  Base<m> b;
//  double lower_bound = 1;
//  double upper_bound = 10;
//  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
//  std::uniform_int_distribution<int> dense_dist(1,dense);
//  std::default_random_engine re;
//  for(int i = 0; i < m; i++)
//    b.base.add_value(i,i,unif(re));
//  for(int i = 0; i < m; i++) {
//    for(int j = 0; j < m; j++) {
//      if(i==j)
//        continue;
//      if(dense_dist(re) == dense)
//        b.base.add_value(i,j,unif(re));
//    }
//  }

