#ifndef SVECTOR_H
#define SVECTOR_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

const double eps = 1e-9;

struct Nonzero {
  int index;
  double value;
};

// Sparse Vector
class SVector {
  public:
    SVector() = default;
    SVector(std::initializer_list<Nonzero>);
    void add_value (int ind, double val);
    int length();

    auto begin() {
      return values.begin();
    }
    auto end() {
      return values.end();
    }

    friend std::ostream &operator<<(std::ostream &os, SVector const &n);

    bool operator==(const SVector& rhs) const;
    bool operator!=(const SVector& rhs) const;

  private:
    std::vector<Nonzero> values;
};

std::ostream &operator<<(std::ostream &os, Nonzero const &n);

#endif
