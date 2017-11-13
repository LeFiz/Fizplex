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

  void updateUnfinishedEtas(size_t i);
  void updateVecWithETM(ETM &etm, SVector &vec);
  void updateVecWithETM(ETM &etm, DVector &vec);
  void swapBaseColumns(size_t i, size_t j);
  bool work_vector_is_zero() const;

  std::unique_ptr<size_t[]> rowOrdering;
  std::vector<std::unique_ptr<ETM>> etms;
  DVector work;

  size_t m;
};

#endif
