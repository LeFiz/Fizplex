#ifndef BASE_H
#define BASE_H

#include "colmatrix.h"
#include <algorithm>
#include <memory>
#include <numeric>

class Base {
public:
  Base() = delete;
  explicit Base(const ColMatrix &);
  void setBase(const ColMatrix &);
  bool invert();
  void updateVec(SVector &vec);
  const ColMatrix &get_base();

private:
  struct ETM {
    SVector eta;
    size_t col;
    ETM() = default;
    ETM(const SVector &e, size_t c) : eta(e), col(c){};
  };

  void updateUnfinishedEtas(size_t i);
  void updateVecWithETM(ETM &etm, SVector &vec);
  void swapBaseColumns(size_t i, size_t j);

  std::unique_ptr<size_t[]> rowOrdering;
  std::vector<std::unique_ptr<ETM>> etms;
  std::unique_ptr<double[]> work;
  ColMatrix base;

  size_t m;
};

#endif
