#include <iostream>
#include <random>

#include "base.h"
#include "colmatrix.h"
#include "lp.h"
#include "svector.h"
#include "test.h"

int main() {
  TestRunner::run();

  LP lp;
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_column(ColType::LowerBound, 0, inf);
  lp.add_row(RowType::LE, -inf, 10);
  lp.add_row(RowType::LE, -inf, 15);

  lp.add_value(0, 0, 3);
  lp.add_value(0, 1, 2);
  lp.add_value(0, 2, 1);
  lp.add_value(1, 0, 2);
  lp.add_value(1, 1, 5);
  lp.add_value(1, 2, 3);

  lp.add_logicals();

  lp.add_obj_value(0, -2);
  lp.add_obj_value(1, -3);
  lp.add_obj_value(2, -4);

  lp.set_b();

  std::cout << "A:\n" << lp.A;
  std::cout << "b:\n";
  for (const auto &val : lp.b)
    std::cout << val << " ";
  std::cout << std::endl;

  std::cout << "c:\n";
  for (const auto &val : lp.c)
    std::cout << val << " ";
  std::cout << std::endl;

  ColMatrix m(2, 0);
  for (size_t i = 0; i < 2; i++)
    m.add_column(lp.A.column(i + 3));
  std::cout << "M:\n" << m << std::endl;

  Base b(m);
  std::cout << "B:\n" << b.get_base() << std::endl;

  b.invert();
  return 0;
}
