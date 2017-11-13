#include "base.h"

Base::Base(const ColMatrix &b) : work(b.row_count()), m(b.row_count()) {
  assert(b.row_count() == b.col_count());
  for (size_t i = 0; i < m; i++)
    etms.push_back(std::make_unique<ETM>(b.column(i), i));
  rowOrdering = std::make_unique<size_t[]>(m);
  for (size_t i = 0; i < m; i++)
    rowOrdering[i] = i;
}

void Base::swapBaseColumns(size_t i, size_t j) {
  assert(i < m && j < m);
  etms[i].swap(etms[j]);
  std::swap<size_t>(rowOrdering[i], rowOrdering[j]);
}

bool Base::invert() {
  assert(work_vector_is_zero());
  double mult;
  for (size_t i = 0; i < m; i++) { // Update all columns
    bool found = false;
    for (size_t j = i; j < m; j++) { // Find non-zero column
      for (auto &n : etms[j]->eta) { // Find right index
        if (n.index == i and not is_zero(n.value)) {
          found = true;
          mult = -n.value;
          n.value = -1.0;
          break;
        }
      }
      if (found) {
        for (auto &n : etms[j]->eta)
          n.value /= mult;
        if (i != j)
          swapBaseColumns(i, j);
        etms[i]->col = i;
        break;
      }
    }
    if (!found)
      return false;
    else
      updateUnfinishedEtas(i);
  }
  assert(work_vector_is_zero());
  return true;
}

bool Base::work_vector_is_zero() {
  auto sum_func = [](double sum, const double val) {
    return sum + std::fabs(val);
  };
  return !(std::accumulate(work.begin(), work.end(), 0.0, sum_func) > 0.0);
}

void Base::updateVecWithETM(ETM &etm, SVector &vec) {

  double mult = 0.0;
  bool found = false;
  // Find nonzero multiplier in vec_i
  for (auto &v : vec) {
    if (v.index == etm.col) {
      found = true;
      mult = v.value;
      v.value =
          0.0; // otherwise v_col = v_col + v_col * mult; should be v_col =
               // v_col * mult
    }
  }
  if (found) { // if !found: v^i_finishedETM is zero, no update required
    for (auto &entry : etm.eta)
      if (entry.value > 0.0 or entry.value < 0.0)
        work[entry.index] = entry.value;
    for (auto &e : vec) {
      if (work[e.index] > 0.0 or work[e.index] < 0.0) {
        e.value += work[e.index] * mult;
        work[e.index] = 0.0;
      }
    }
    for (auto &e : etm.eta) {
      if (work[e.index] > 0.0 or work[e.index] < 0.0) {
        vec.add_value(e.index, work[e.index] * mult);
        work[e.index] = 0.0;
      }
    }
  }
}

void Base::updateVecWithETM(ETM &etm, DVector &vec) {
  assert(vec.dimension() == m);
  double mult = vec[etm.col];
  vec[etm.col] = 0.0; // special case for pivot

  for (auto &entry : etm.eta)
    vec[entry.index] += entry.value * mult;
}

void Base::ftran(SVector &vec) {
  for (const auto &etm : etms)
    updateVecWithETM(*etm, vec);

  // Reorder vec according to base column swaps
  for (auto &entry : vec)
    entry.index = rowOrdering[entry.index];
}

void Base::ftran(DVector &vec) {
  for (auto &etm : etms)
    updateVecWithETM(*etm, vec);

  // Reorder vec according to base column swaps
  DVector w(m);
  for (size_t i = 0; i < m; i++)
    w[rowOrdering[i]] = vec[i];
  vec = w;
}

void Base::btran(DVector &vec) const {
  const size_t num = etms.size();
  for (size_t i = 0; i < num; i++) {
    double d = 0.0;
    for (const auto &n : etms[num - i - 1]->eta) {
      d += n.value * vec[rowOrdering[n.index]];
    }
    vec[rowOrdering[etms[num - i - 1]->col]] = d;
  }

  // Reorder vec according to base column swaps
  DVector w(m);
  for (size_t i = 0; i < m; i++)
    w[i] = vec[rowOrdering[i]];
  vec = w;
}

void Base::updateUnfinishedEtas(size_t finishedETM) {
  // Update v_i in that are saved in etas
  for (auto i = finishedETM + 1; i < etms.size(); i++)
    updateVecWithETM(*etms[finishedETM], etms[i]->eta);
}
