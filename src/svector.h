#ifndef SVECTOR_H
#define SVECTOR_H

#include <vector>
#include <iostream>

struct Nonzero {
  int index;
  double value;
};

// Sparse Vector
class SVector {
  public:
    void add_value (int ind, double val);
    int length();

    using iterator = std::vector<Nonzero>::iterator;
    iterator begin() {
      return values.begin();
    }
    iterator end() {
      return values.end();
    }

    friend std::ostream &operator<<(std::ostream &os, SVector const &n);
  private:
    std::vector<Nonzero> values;
};

std::ostream &operator<<(std::ostream &os, Nonzero const &n);

#endif
