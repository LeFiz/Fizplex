#ifndef COLMATRIX_H
#define COLMATRIX_H

#include <array>
#include <memory>
#include "svector.h"


template <int m, int n = m>
class ColMatrix {
  private:
    std::array<std::unique_ptr<SVector>, n> cols;
  public:
    ColMatrix() {
      for(auto& i : cols)
        i = std::make_unique<SVector>();
    }
    void add_value(int row, int col, double val) {
      cols[col]->add_value(row, val);
    }
    SVector& column(int i) {
      return *cols[i];
    }
    template <int mm, int nn>
    friend std::ostream &operator<<(std::ostream &os, ColMatrix<mm,nn> const &matrix);
};

template <int m, int n>
std::ostream &operator<<(std::ostream &os, ColMatrix<m,n> const &matrix) {
  for(auto& col : matrix.cols)
    os << *col << std::endl;
  return os;
}

#endif
