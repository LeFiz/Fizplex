#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "lp.h"

class Simplex {
public:
  enum class Result { Infeasible, OptimalSolution, Unbounded, Unsolved };

  explicit Simplex(const LP &_lp);
  void solve();

  const DVector &get_x() const;
  const double &get_z() const;
  const Result &get_result() const;

private:
  const LP &lp;
  DVector x;
  double z;
  Result result;
  size_t structural_count;
  size_t row_count;
  size_t col_count;
};

#endif
