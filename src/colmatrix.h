#ifndef COLMATRIX_H
#define COLMATRIX_H

#include <array>
#include <memory>
#include <cassert>
#include "svector.h"


template <size_t m, size_t n = m>
class ColMatrix {
  private:
    std::array<std::unique_ptr<SVector>, n> cols;
  public:
    ColMatrix() {
      for(auto& i : cols)
        i = std::make_unique<SVector>();
    }
    ColMatrix(std::initializer_list<SVector>);
    void add_value(size_t row, size_t col, double val) {
      cols[col]->add_value(row, val);
    }
    const SVector& column(size_t i) const {
      return *cols[i];
    }
    void swapColumns(size_t i, size_t j);
    ColMatrix& operator=(ColMatrix& rhs) {
      for(size_t i = 0; i < n; i++)
        cols[i] = std::make_unique<SVector>(rhs.column(i));
      return *this;
    }
    bool operator==(const ColMatrix& rhs) const;
    bool operator!=(const ColMatrix& rhs) const;
    template <size_t mm, size_t nn>
    friend std::ostream &operator<<(std::ostream &os, ColMatrix<mm,nn> const &matrix);
};

template <size_t m, size_t n>
std::ostream &operator<<(std::ostream &os, ColMatrix<m,n> const &matrix) {
  for(auto& col : matrix.cols)
    os << *col << std::endl;
  return os;
}

template <size_t m, size_t n>
ColMatrix<m,n>::ColMatrix(std::initializer_list<SVector> colList) {
  assert(colList.size() == n);
  size_t i = 0;
  for(auto& v : colList)
    cols[i++] = std::make_unique<SVector>(v);
}

template <size_t m, size_t n>
void ColMatrix<m,n>::swapColumns(size_t i, size_t j) {
  assert(i < m && j < m);
  cols[i].swap(cols[j]);
}

template <size_t m, size_t n>
bool ColMatrix<m,n>::operator==(const ColMatrix& rhs) const {
  for(size_t i = 0; i < n; i++) {
    if(column(i) != rhs.column(i))
      return false;
  }
  return true;
}

template <size_t m, size_t n>
bool ColMatrix<m,n>::operator!=(const ColMatrix& rhs) const {
  return !(*this == rhs);
}

#endif
