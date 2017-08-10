#include "svector.h"


int main() {
  SVector vec;
  vec.add_value(2, 1.234);
  vec.add_value(4, -0.0274);
  vec.add_value(101, 4.34);
  vec.add_value(103, -1);
  std::cout << vec << std::endl;
  return 0;
}
