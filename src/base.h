#ifndef BASE_H
#define BASE_H

#include "colmatrix.h"

using Vec = std::vector<Nonzero>;

template <int m>
class Base {
  public:
    Base();
    ColMatrix<m> base;
    void invert();
    void updateUnfinishedEtas(int i);
  private:
    struct ETM {
      Vec eta;
      int col;
    };
    std::vector<ETM> etms;
    std::array<double,m> work;
};


template <int m>
Base<m>::Base() {
  work.fill(0.0);
};


template <int m>
void Base<m>::invert() {
  etms.clear();
  etms.reserve(m);
  for(int i = 0; i < m; i++)
    etms.push_back({base.column(i), i});
  double mult;
  for(int i = 0; i < m; i++) {
    for(auto& n : etms[i].eta) {
      if(n.index == i) {
        mult =  - 1.0 / n.value;
        n.value = -1;
        std::cout << "Mult = " << mult << std::endl;
        break;
      }
    }
    for(auto& n : etms[i].eta) {
      n.value *= mult;
    }
    std::cout << "Eta:" << std::endl;
    for(auto j = 0; j < m; j++)
      std::cout << etms[j].eta;

    // Update v_i
    updateUnfinishedEtas(i);

    std::cout << "Eta:" << std::endl;
    for(auto j = 0; j < m; j++)
      std::cout << etms[j].eta;
  }
};

template <int m>
void Base<m>::updateUnfinishedEtas(int finishedETM) {
  double mult = 0.0;
  bool found;
  int posi = -1;
  // Copy finishedETM eta vector to work array
  for(const auto& entry : etms[finishedETM].eta)
    work[entry.index] = entry.value;

  // Update v_i in that are saved in etas
  for(int i = finishedETM + 1; i < etms.size(); i++) {
    found = false;
  // Find nonzero multiplier in v_i
    for(auto& v : etms[i].eta) {
      if(v.index == finishedETM) {
        found = true;
        mult = v.value;
        posi = v.index;
      }
    }
    if(found) { // if !found: v^i_finishedETM is zero, no update required
      for(auto& v : etms[i].eta)
        v.value += mult * work[v.index];

      // v_i = v_i + eta_i * v_i except for pivot element, there it's v_i = eta_i * v_i, i = posi
      etms[i].eta[posi].value -= mult;
    }
  }
  // Reset work to 0
  for(const auto& entry : etms[finishedETM].eta)
    work[entry.index] = 0.0;
};

std::ostream& operator<<(std::ostream& os, Vec const& vec) {
  for(const auto& n : vec)
    std::cout << n;
  std::cout << std::endl;
}

#endif
