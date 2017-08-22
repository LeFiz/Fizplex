#ifndef BASE_H
#define BASE_H

#include "colmatrix.h"
#include <algorithm>
#include <numeric>


template <int m>
class Base {
  public:
    Base();
    explicit Base(const ColMatrix<m>&);
    void setBase(const ColMatrix<m>&);
    bool invert();
    void updateVec(SVector& vec);
  private:
    struct ETM {
      SVector eta;
      size_t col;
      ETM() = default;
      ETM(const SVector& e, size_t c) : eta(e), col(c) {};
    };

    void updateUnfinishedEtas(size_t i);
    void updateVecWithETM(ETM& etm, SVector& vec);
    void swapBaseColumns(size_t i, size_t j);

    std::array<int,m> rowOrdering;
    std::vector<std::unique_ptr<ETM>> etms;
    std::array<double,m> work;
    ColMatrix<m> base;
};


template <int m>
Base<m>::Base()
  : work()
{
  setBase(ColMatrix<m>());
}

template <int m>
Base<m>::Base(const ColMatrix<m>& b)
  : work()
{
  setBase(b);
}

template <int m>
void Base<m>::setBase(const ColMatrix<m>& b) {
  base = b;
  for(auto i = 0; i < m; i++)
    rowOrdering[i]=i;
  etms.clear();
}

template <int m>
void Base<m>::swapBaseColumns(size_t i, size_t j) {
  assert(i < m && j < m);
  etms[i].swap(etms[j]);
  base.swapColumns(i,j);
  std::swap<int>(rowOrdering[i], rowOrdering[j]);
}

template <int m>
bool Base<m>::invert() {
  etms.clear();
  etms.reserve(m);
  for(auto i = 0; i < m; i++)
    etms.push_back(std::make_unique<ETM>(base.column(i), i));
  double mult;
  for(size_t i = 0; i < m; i++) { // Update all columns
    auto found = false;
    for(size_t j = i; j < m; j++) { // Find non-zero column
      for(auto& n : etms[j]->eta) { // Find right index
        if(n.index == i) {
          if(is_zero(n.value))
            break;
          found = true;
          mult =  - 1.0 / n.value;
          n.value = -1;
          break;
        }
      }
      if(found) {
        for(auto& n : etms[j]->eta)
          n.value *= mult;
        etms[j]->col = i;
        if(i!=j) {
          swapBaseColumns(i,j);
        }
        break;
      }
    }
    if(!found)
      return false;
    updateUnfinishedEtas(i);
  }
  return true;
}

template <int m>
void Base<m>::updateVecWithETM(ETM& etm, SVector& vec) {
  assert(std::accumulate(work.begin(), work.end(), 0) == 0); //quite slow

  double mult = 0.0;
  bool found = false;
  // Find nonzero multiplier in vec_i
  for(auto& v : vec) {
    if(v.index == etm.col) {
      found = true;
      mult = v.value;
      v.value = 0; // otherwise v_col = v_col + v_col * mult; should be v_col = v_col * mult
    }
  }
  if(found) { // if !found: v^i_finishedETM is zero, no update required
    for(auto& entry : etm.eta)
      work[entry.index] = entry.value;
    for(auto& e : vec) {
      if(!is_zero(work[e.index])) {
        e.value += work[e.index] * mult;
        work[e.index] = 0.0;
      }
    }
    for(auto& e : etm.eta) {
      if(!is_zero(work[e.index])) {
        vec.add_value(e.index, work[e.index] * mult);
        work[e.index] = 0.0;
      }
    }
  }

  assert(std::accumulate(work.begin(), work.end(), 0) == 0);
}

template <int m>
void Base<m>::updateVec(SVector& vec) {
  for(auto& etm : etms)
    updateVecWithETM(*etm, vec);

  // Reorder vec according to base column swaps
  SVector w;
  for(auto& entry : vec)
    w.add_value(rowOrdering[entry.index], entry.value);
  vec = w;
}

template <int m>
void Base<m>::updateUnfinishedEtas(size_t finishedETM) {
  // Update v_i in that are saved in etas
  for(auto i = finishedETM + 1; i < etms.size(); i++)
    updateVecWithETM(*etms[finishedETM], etms[i]->eta);

}

#endif
