#include "svector.h"
#include "colmatrix.h"


int main() {
  ColMatrix<3> m;
  m.add_value(0,0,7);
  m.add_value(0,1,2);
  m.add_value(0,2,1);
  m.add_value(1,0,0);
  m.add_value(1,1,3);
  m.add_value(1,2,-1);
  m.add_value(2,0,-3);
  m.add_value(2,1,4);
  m.add_value(2,2,-2);
  std::cout << m;
  return 0;
}
