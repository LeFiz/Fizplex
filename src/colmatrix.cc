#include "colmatrix.h"

ColMatrix::ColMatrix() : m(0), n(0) {}

ColMatrix::ColMatrix(size_t _m, size_t _n) : m(_m), n(_n) {
  for (size_t i = 0; i < n; i++)
    cols.push_back(SVector());
}

void ColMatrix::add_value(size_t row, size_t col, double val) {
  cols[col].add_value(row, val);
}

size_t ColMatrix::row_count() const { return m; }
size_t ColMatrix::col_count() const { return n; }

const SVector &ColMatrix::column(size_t i) const {
  assert(i < n);
  return cols[i];
}

SVector &ColMatrix::column(size_t i) {
  assert(i < n);
  return cols[i];
}

std::ostream &operator<<(std::ostream &os, ColMatrix const &matrix) {
  for (auto &col : matrix.cols)
    os << col << std::endl;
  return os;
}

ColMatrix::ColMatrix(size_t _m, size_t _n,
                     std::initializer_list<SVector> colList)
    : m(_m), n(_n) {
  assert(colList.size() <= n);
  for (auto &v : colList)
    cols.push_back(v);
}

void ColMatrix::swap_columns(size_t i, size_t j) {
  assert(i < m && j < m);
  std::swap<SVector>(cols[i], cols[j]);
}

bool ColMatrix::operator==(const ColMatrix &rhs) const {
  if (m != rhs.m || n != rhs.n)
    return false;
  for (size_t i = 0; i < n; i++) {
    if (column(i) != rhs.column(i))
      return false;
  }
  return true;
}

bool ColMatrix::operator!=(const ColMatrix &rhs) const {
  return !(*this == rhs);
}

ColMatrix &ColMatrix::operator=(const ColMatrix &rhs) {
  m = rhs.m;
  n = rhs.n;
  cols.clear();
  for (size_t i = 0; i < n; i++)
    cols.push_back(rhs.column(i));
  return *this;
}

void ColMatrix::add_row() { m++; }

void ColMatrix::add_column(const SVector &v) {
  assert(n == cols.size());
  n++;
  cols.push_back(v);
}

double ColMatrix::get_value(size_t row, size_t column) const {
  assert(column < n && row < m);
  return cols[column].get_value(row);
}

ColMatrix ColMatrix::identity(size_t dim) {
  ColMatrix m(dim, dim);
  for (size_t i = 0; i < dim; ++i)
    m.add_value(i, i, 1.0);
  return m;
}
