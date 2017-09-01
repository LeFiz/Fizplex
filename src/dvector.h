#ifndef DVECTOR_H
#define DVECTOR_H

#include <valarray>

class DVector {
public:
  explicit DVector(size_t dimension = 0);

  const size_t &dimension() const;
  bool operator==(const DVector &rhs) const;
  bool operator!=(const DVector &rhs) const;
  double &operator[](size_t i);
  const double &operator[](size_t i) const;

  double operator*(const DVector &rhs) const;

private:
  size_t dim;
  std::valarray<double> vals;
};

#endif
