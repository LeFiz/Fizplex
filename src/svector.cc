#include "svector.h"
#include <typeinfo>

void SVector::add_value (int ind, double val) {
  values.push_back({ind, val});
}

int SVector::length() {
  return values.size();
}

std::ostream &operator<<(std::ostream &os, Nonzero const &n) {
  return os << "(" << n.index << "," << n.value << ")";
}

std::ostream & operator<<(std::ostream &os, SVector const &vec) {
  for (const auto &n : vec.values) {
    os << n;
  }
  return os;
}

