#ifndef COLMATRIX_H
#define COLMATRIX_H

#include <array>
#include <memory>
#include <cassert>
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
    ColMatrix(std::initializer_list<SVector>);
    void add_value(int row, int col, double val) {
      cols[col]->add_value(row, val);
    }
    const SVector& column(int i) const {
      return *cols[i];
    }
    ColMatrix& operator=(ColMatrix& rhs) {
      for(int i = 0; i < n; i++)
        cols[i] = std::make_unique<SVector>(rhs.column(i));
      return *this;
    }
    bool operator==(const ColMatrix& rhs) const;
    bool operator!=(const ColMatrix& rhs) const;
    template <int mm, int nn>
    friend std::ostream &operator<<(std::ostream &os, ColMatrix<mm,nn> const &matrix);
};

template <int m, int n>
std::ostream &operator<<(std::ostream &os, ColMatrix<m,n> const &matrix) {
  for(auto& col : matrix.cols)
    os << *col << std::endl;
  return os;
}

template <int m, int n>
ColMatrix<m,n>::ColMatrix(std::initializer_list<SVector> colList) {
  assert(colList.size() == n);
  int i = 0;
  for(auto& v : colList)
    cols[i++] = std::make_unique<SVector>(v);
}

template <int m, int n>
bool ColMatrix<m,n>::operator==(const ColMatrix& rhs) const {
  for(int i = 0; i < n; i++) {
    if(column(i) != rhs.column(i))
      return false;
  }
  return true;
}

template <int m, int n>
bool ColMatrix<m,n>::operator!=(const ColMatrix& rhs) const {
  return !(*this == rhs);
}

#endif
