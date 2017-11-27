#include "svector.h"
#include <cassert>
#include <typeinfo>

namespace fizplex {

SVector::SVector() : values() {}

SVector::SVector(std::initializer_list<Nonzero> l) : values(l) {}

void SVector::add_value(size_t ind, double val) {
  values.push_back({ind, val});
}

size_t SVector::length() const { return values.size(); }

bool SVector::operator==(const SVector &rhs) const {
  int lhsNonzeroEntries = 0, rhsNonzeroEntries = 0;
  for (const auto &e : values) {
    if (!is_zero(e.value))
      lhsNonzeroEntries++;
  }
  for (const auto &e : rhs.values) {
    if (!is_zero(e.value))
      rhsNonzeroEntries++;
  }
  if (lhsNonzeroEntries != rhsNonzeroEntries)
    return false;
  for (const auto &lhsVal : values) {
    if (!is_zero(lhsVal.value)) {
      bool found = false;
      for (const auto &rhsVal : rhs.values) {
        if (lhsVal.index == rhsVal.index &&
            is_eq_norm(lhsVal.value, rhsVal.value)) {
          found = true;
          break;
        }
      }
      if (!found)
        return false;
    }
  }
  return true;
}

bool SVector::operator!=(const SVector &rhs) const { return !(*this == rhs); }

std::ostream &operator<<(std::ostream &os, Nonzero const &n) {
  return os << "(" << n.index << "," << n.value << ")";
}

std::ostream &operator<<(std::ostream &os, SVector const &vec) {
  for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
    os << *it;
  }
  return os;
}

double SVector::get_value(size_t ind) const {
  for (auto &n : values) {
    if (n.index == ind)
      return n.value;
  }
  return 0.0;
}

double &SVector::operator[](size_t ind) {
  for (size_t i = 0; i < values.size(); ++i) {
    if (values[i].index == ind)
      return values[i].value;
  }
  add_value(ind, 0.0);
  return values.back().value;
}

SVector operator*(const double c, const SVector &vec) {
  SVector result = vec;
  for (auto &n : result)
    n.value *= c;
  return result;
}

} // namespace fizplex
