#ifndef BASE_H
#define BASE_H

#include "colmatrix.h"
#include "dvector.h"
#include <algorithm>
#include <memory>
#include <numeric>

class Base {
public:
  Base() = delete;
  explicit Base(const ColMatrix &);
  bool invert();
  ColMatrix get_inverse();
  void ftran(SVector &vec);
  void ftran(DVector &vec);
  void btran(DVector &vec) const;

private:
  struct ETM {
    SVector eta;
    size_t col;
    ETM() = default;
    ETM(const SVector &e, size_t c) : eta(e), col(c){};
  };
  struct Pivot {
    bool found;
    double value;
    size_t index;
  };
  void apply_etm(ETM &etm, SVector &vec);
  void apply_etm(ETM &etm, DVector &vec);
  void swap_columns(size_t i, size_t j);
  bool work_vector_is_zero() const;
  Pivot find_pivot(size_t) const;

  std::vector<ETM> etms;
  DVector work;

  size_t m;
  std::unique_ptr<size_t[]> row_ordering;
};

#endif
