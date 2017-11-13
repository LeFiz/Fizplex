#include "base.h"

Base::Base(const ColMatrix &b) : work(b.row_count()), m(b.row_count()) {
  assert(b.row_count() == b.col_count());
  for (size_t i = 0; i < m; i++)
    etms.push_back(ETM(b.column(i), i));
  row_ordering = std::make_unique<size_t[]>(m);
  for (size_t i = 0; i < m; i++)
    row_ordering[i] = i;
}

void Base::swap_columns(size_t i, size_t j) {
  assert(i < m && j < m);
  std::swap(etms[i], etms[j]);
  std::swap<size_t>(row_ordering[i], row_ordering[j]);
}

bool Base::invert() {
  assert(work_vector_is_zero());
  for (size_t i = 0; i < m; i++) { // Update all columns
    bool found = false;
    for (size_t j = i; j < m; j++) { // Find non-zero column
      for (auto &n : etms[j].eta) {  // Find right index
        if (n.index == i and not is_zero(n.value)) {
          found = true;
          const double mult = -n.value;
          n.value = -1.0;
          for (auto &v : etms[j].eta)
            v.value /= mult;
          if (i != j)
            swap_columns(i, j);
          etms[i].col = i;
          for (auto k = i + 1; k < etms.size(); k++)
            apply_etm(etms[i], etms[k].eta);
          break;
        }
      }
      if (found)
        break;
    }
    if (!found)
      return false;
  }
  assert(work_vector_is_zero());
  return true;
}

bool Base::work_vector_is_zero() const {
  static const auto sum_func = [](const double sum, const double val) {
    return sum + std::fabs(val);
  };
  return !(std::accumulate(work.cbegin(), work.cend(), 0, sum_func) > 0.0);
}

void Base::apply_etm(ETM &etm, SVector &vec) {

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

void Base::apply_etm(ETM &etm, DVector &vec) {
  assert(vec.dimension() == m);
  double mult = vec[etm.col];
  vec[etm.col] = 0.0; // special case for pivot

  for (auto &entry : etm.eta)
    vec[entry.index] += entry.value * mult;
}

void Base::ftran(SVector &vec) {
  for (auto &etm : etms)
    apply_etm(etm, vec);

  // Reorder vec according to base column swaps
  for (auto &entry : vec)
    entry.index = row_ordering[entry.index];
}

void Base::ftran(DVector &vec) {
  for (auto &etm : etms)
    apply_etm(etm, vec);

  // Reorder vec according to base column swaps
  DVector w(m);
  for (size_t i = 0; i < m; i++)
    w[row_ordering[i]] = vec[i];
  vec = w;
}

void Base::btran(DVector &vec) const {
  const size_t num = etms.size();
  for (size_t i = num; i-- > 0;) {
    double d = 0.0;
    for (auto it = etms[i].eta.cbegin(); it != etms[i].eta.cend(); ++it)
      d += it->value * vec[row_ordering[it->index]];
    vec[row_ordering[etms[i].col]] = d;
  }

  // Reorder vec according to base column swaps
  DVector w(m);
  for (size_t i = 0; i < m; i++)
    w[i] = vec[row_ordering[i]];
  vec = w;
}
