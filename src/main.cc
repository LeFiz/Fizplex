#include <random>

#include "svector.h"
#include "colmatrix.h"
#include "base.h"


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
  Base<3> b;
  b.base.add_value(0,0,7);
  b.base.add_value(0,1,2);
  b.base.add_value(0,2,1);
  b.base.add_value(1,1,3);
  b.base.add_value(1,2,-1);
  b.base.add_value(2,0,-3);
  b.base.add_value(2,1,4);
  b.base.add_value(2,2,-2);
  std::cout << "Base:\n";
  std::cout << b.base;
  b.invert();
  return 0;
}
