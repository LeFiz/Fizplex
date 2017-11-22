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
  etms[i].col = i;
  etms[j].col = j;
  std::swap<size_t>(row_ordering[i], row_ordering[j]);
}

Base::Pivot Base::find_pivot(size_t ind) const {
  static const auto dist_to_one = [](double d) {
    return std::fabs(1 - 1.0 / std::fabs(d));
  };
  Pivot pivot{false, 0, 0};
  for (size_t j = ind; j < m; j++) { // Find etm with non-zero in ind
    auto val = etms[j].eta.get_value(ind);
    if (is_zero(val))
      continue;
    if (not pivot.found or (dist_to_one(val) < dist_to_one(pivot.value))) {
      pivot.found = true;
      pivot.index = j;
      pivot.value = val;
    }
  }
  return pivot;
}

bool Base::invert() {
  assert(work_vector_is_zero());

  for (size_t i = 0; i < m; i++) { // Update all columns
    auto pivot = find_pivot(i);
    if (not pivot.found)
      return false;

    for (auto &v : etms[pivot.index].eta) {
      if (v.index == i)
        v.value = 1.0 / pivot.value;
      else
        v.value /= -pivot.value;
    }
    if (i != pivot.index)
      swap_columns(i, pivot.index);

    for (auto k = i + 1; k < etms.size(); k++)
      apply_etm(etms[i], etms[k].eta);
  } // for i ...

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
  if (found) {
    for (auto &entry : etm.eta)
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
  DVector w(m);
  for (size_t i = 0; i < m; i++)
    w[i] = vec[row_ordering[i]];

  for (size_t i = etms.size(); i-- > 0;) {
    double d = 0.0;
    for (auto it = etms[i].eta.cbegin(); it != etms[i].eta.cend(); ++it)
      d += it->value * w[it->index];
    w[etms[i].col] = d;
  }

  vec = w;
}

ColMatrix Base::get_inverse() {
  auto inv = ColMatrix::identity(m);
  for (size_t ind = 0; ind < m; ++ind) {
    for (size_t i = 0; i < m; ++i)
      apply_etm(etms[ind], inv.column(i));
  }

  for (size_t col = 0; col < m; ++col) {
    for (auto &n : inv.column(col))
      n.index = row_ordering[n.index];
  }
  return inv;
}
