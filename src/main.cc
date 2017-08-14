#include "svector.h"
#include "colmatrix.h"
#include "base.h"


int main() {
  Base<2> b;
  b.base.add_value(0,0,1);
  b.base.add_value(0,1,0);
  b.base.add_value(1,0,3);
  b.base.add_value(1,1,2);
//  Base<3> b;
//  b.base.add_value(0,0,-24);
//  b.base.add_value(0,1,18);
//  b.base.add_value(0,2,5);
//  b.base.add_value(1,0,20);
//  b.base.add_value(1,1,-15);
//  b.base.add_value(1,2,-4);
//  b.base.add_value(2,0,-5);
//  b.base.add_value(2,1,4);
//  b.base.add_value(2,2,1);
  std::cout << "Base:\n";
  std::cout << b.base;
  b.invert();
  return 0;
}
