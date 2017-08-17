#include <random>
#include <iostream>

#include "svector.h"
#include "colmatrix.h"
#include "base.h"

#define EXPECT_TRUE(statement) (statement ? std::cerr : (std::cerr << "Error! Expected true got false! In " << __FUNCTION__ << " (Line " << __LINE__ << " in " << __FILE__ << ")\n"))

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

int main() {
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
//  Base<2> b;
//  b.base.add_value(0,0,1);
//  b.base.add_value(0,1,0);
//  b.base.add_value(1,0,3);
//  b.base.add_value(1,1,2);
  test_svector();
  test_colmatrix();
  ColMatrix<3> m = {{{0,7}, {2,-3}},
                    {{0,2}, {1,3}, {2,4}},
                    {{0,1}, {1,-1}, {2,-2}}};
  Base<3> b;
  b.base = m;
  std::cout << "Base:\n";
  std::cout << b.base;
  b.invert();
  SVector v = {{0, 1}};
  b.updateVec(v);
  std::cout << v << std::endl;
  SVector w = {{0,-2}, {1,3}, {2,9}, {7,0}};
  std::cout << w << std::endl;
  std::cout << (v==w) << std::endl;
  return 0;
}
