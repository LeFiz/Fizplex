#include "svector.h"
#include <typeinfo>

SVector::SVector() :
  values()
{
}

SVector::SVector(std::initializer_list<Nonzero> l) :
  values(l)
{
}

void SVector::add_value (size_t ind, double val) {
  values.push_back({ind, val});
}

size_t SVector::length() {
  return values.size();
}

bool SVector::operator==(const SVector& rhs) const {
  int lhsNonzeroEntries = 0, rhsNonzeroEntries = 0;
  for(const auto& e : values)
  {
    if(!is_eq(e.value, 0.0))
      lhsNonzeroEntries++;
  }
  for(const auto& e : rhs.values)
  {
    if(!is_eq(e.value, 0.0))
      rhsNonzeroEntries++;
  }
  if(lhsNonzeroEntries != rhsNonzeroEntries)
    return false;
  bool found;
  for(const auto& lhsVal : values) {
    if(is_eq(lhsVal.value, 0.0))
      continue;
    found = false;
    for(const auto& rhsVal : rhs.values) {
      if(lhsVal.index == rhsVal.index && fabs(lhsVal.value - rhsVal.value) < eps) {
        found=true;
        break;
      }
    }
    if(!found)
      return false;
  }
  return true;
}

bool SVector::operator!=(const SVector& rhs) const {
  return !(*this == rhs);
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


