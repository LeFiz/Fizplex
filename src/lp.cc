#include "lp.h"
#include <cassert>

LP::LP() : A(), b(), c(), cols(), rows() {}

LP::Column::Column(ColType _type, double _lower, double _upper,
                   bool _is_logical)
    : type(_type), is_logical(_is_logical), lower(_lower), upper(_upper) {}

void LP::add_column(ColType type, double lower, double upper, double obj_value,
                    bool is_logical) {
  cols.push_back(Column(type, lower, upper, is_logical));
  A.add_column();
  c.append(obj_value);
}

void LP::add_column(ColType _type, double _lower, double _upper,
                    double obj_value) {
  add_column(_type, _lower, _upper, obj_value, false);
}

void LP::add_logical_column(ColType _type, double _lower, double _upper) {
  add_column(_type, _lower, _upper, 0.0f, true);
}

LP::Row::Row(RowType _type, double _lower, double _upper)
    : type(_type), lower(_lower), upper(_upper) {}

void LP::add_row(RowType _type, double _lower, double _upper) {
  rows.push_back(Row(_type, _lower, _upper));
  A.add_row();
  add_b();
}

size_t LP::column_count() const { return cols.size(); }

size_t LP::row_count() const { return rows.size(); }

void LP::add_value(size_t row, size_t column, double value) {
  assert(row < row_count());
  assert(column < column_count());

  A.add_value(row, column, value);
}

double LP::get_value(size_t row, size_t column) const {
  assert(row < row_count());
  assert(column < column_count());

  return A.get_value(row, column);
}

void LP::add_b() {
  const Row &row = rows.back();
  switch (row.type) {
  case RowType::GE:
    b.append(row.lower);
    break;
  case RowType::NonBinding:
    b.append(0.0f);
    break;
  default:
    b.append(row.upper);
  }
}

void LP::add_obj_value(size_t ind, double d) {
  assert(ind < c.dimension());
  c[ind] = d;
}

double LP::get_obj_value(size_t ind) const {
  assert(ind < c.dimension());
  return c[ind];
}

void LP::add_logicals() {
  const size_t first_logical_index = column_count();
  for (size_t i = 0; i < row_count(); i++) {

    switch (rows[i].type) {
    case RowType::Equality:
      assert(is_finite(rows[i].upper));
      assert(is_finite(rows[i].lower));
      assert(is_eq(rows[i].upper, rows[i].lower));
      add_logical_column(ColType::Fixed, 0.0, 0.0);
      break;
    case RowType::Range:
      assert(is_finite(rows[i].upper));
      assert(is_finite(rows[i].lower));
      add_logical_column(ColType::Bounded, 0.0, rows[i].upper - rows[i].lower);
      break;
    case RowType::LE:
      assert(is_finite(rows[i].upper));
      add_logical_column(ColType::LowerBound, 0.0, inf);
      break;
    case RowType::NonBinding:
      add_logical_column(ColType::Free, -inf, inf);
      break;
    case RowType::GE:
      assert(is_finite(rows[i].lower));
      add_logical_column(ColType::UpperBound, -inf, 0);
      break;
    default:
      break;
    }
    add_value(i, i + first_logical_index, 1.0);
  }
}

const LP::Column &LP::column_header(size_t column) const {
  assert(column < cols.size());
  return cols[column];
}

bool LP::is_feasible(const DVector &x) const {
  assert(x.dimension() == column_count());
  for (size_t i = 0; i < cols.size(); i++) {
    if ((is_finite(cols[i].lower) && is_lower(x[i], cols[i].lower)) ||
        (is_finite(cols[i].upper) && is_greater(x[i], cols[i].upper))) {
      return false;
    }
  }
  for (size_t row = 0; row < row_count(); row++) {
    double val = 0.0f;
    for (size_t col = 0; col < column_count(); col++) {
      val += A.get_value(row, col) * x[col];
    }
    if ((is_finite(rows[row].lower) && is_lower(val, rows[row].lower)) ||
        (is_finite(rows[row].upper) && is_greater(val, rows[row].upper))) {
      return false;
    }
  }
  return true;
}
