#ifndef COLMATRIX_H
#define COLMATRIX_H

#include "svector.h"
#include <cassert>
#include <memory>
#include <vector>

class ColMatrix {
public:
  ColMatrix();
  ColMatrix(size_t _m, size_t _n);
  ColMatrix(size_t _m, size_t _n, std::initializer_list<SVector>);
  ColMatrix &operator=(const ColMatrix &rhs);

  size_t row_count() const;
  size_t col_count() const;
  const SVector &column(size_t i) const;
  double get_value(size_t row, size_t column);
  void add_row();
  void add_column(const SVector &v = SVector());
  void add_value(size_t row, size_t col, double val);
  void swap_columns(size_t i, size_t j);

  bool operator==(const ColMatrix &rhs) const;
  bool operator!=(const ColMatrix &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, ColMatrix const &matrix);

private:
  std::vector<std::unique_ptr<SVector>> cols;
  size_t m, n;
};

#endif
