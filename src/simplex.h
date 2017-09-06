#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "lp.h"

class Simplex {
public:
  explicit Simplex(const LP &_lp);
  void solve();

private:
  const LP &lp;
  DVector x;
  double z;
  const size_t structural_count;
  const size_t row_count;
  const size_t col_count;
};

#endif
