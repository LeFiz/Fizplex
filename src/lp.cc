#include "lp.h"
#include "debug.h"
#include <cassert>
#include <unordered_map>

namespace fizplex {

LP::LP() : A(), b(), c(), cols(), rows() {}

LP::LP(const LP& other)
: A(other.A)
, b(other.b)
, c(other.c)
, cols(other.cols)
, rows(other.rows)
{}

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
  add_column(_type, _lower, _upper, 0.0, true);
}

LP::Row::Row(RowType _type, double _lower, double _upper)
    : type(_type), lower(_lower), upper(_upper) {}

void LP::add_row(RowType _type, double _lower, double _upper) {
  add_row(Row(_type, _lower, _upper));
}

void LP::add_row(RowType type, double lower, double upper, DVector row) {
  assert(row.dimension() == column_count());
  add_row(type, lower, upper);
  auto row_index = row_count() - 1;
  for (size_t col_index = 0; col_index < row.dimension(); col_index++) {
    add_value(row_index, col_index, row[col_index]);
  }
}

void LP::add_row(Row row) {
  rows.push_back(row);
  A.add_row();
  b.resize(rows.size());
  set_b(rows.size() - 1);
}

void LP::update_row_header(size_t ind, RowType type, double lower,
                           double upper) {
  rows[ind] = Row(type, lower, upper);
  set_b(ind);
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

void LP::set_b(size_t ind) {
  const Row &row = rows[ind];
  switch (row.type) {
  case RowType::GE:
    b[ind] = row.lower;
    break;
  case RowType::NonBinding:
    b[ind] = 0.0;
    break;
  default:
    b[ind] = row.upper;
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
      assert(
          is_finite(rows[i].upper)); // TODO Move to general is_consistent func
      assert(is_finite(rows[i].lower));
      assert(is_eq_norm(rows[i].upper, rows[i].lower));
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

LP::Column &LP::column_header(size_t column) {
  assert(column < cols.size());
  return cols[column];
}

const LP::Column &LP::column_header(size_t column) const {
  assert(column < cols.size());
  return cols[column];
}

const LP::Row &LP::row_header(size_t row) const {
  assert(row < rows.size());
  return rows[row];
}

bool LP::is_feasible(const DVector &x) const {
  assert(x.dimension() == cols.size());
  bool feasible = true;
  for (size_t i = 0; i < cols.size(); i++) {
    if (is_finite(cols[i].lower) && is_lower_norm(x[i], cols[i].lower)) {
      Debug(0) << "Var " << i << " is below lower bound: " << x[i] << " < "
               << cols[i].lower << "\n";
      feasible = false;
    }
    if (is_finite(cols[i].upper) && is_greater_norm(x[i], cols[i].upper)) {
      Debug(0) << "Var " << i << " is above upper bound: " << x[i] << " > "
               << cols[i].upper << "\n";
      feasible = false;
    }
  }
  for (size_t row = 0; row < row_count(); row++) {
    double val = 0.0;
    for (size_t col = 0; col < column_count(); col++)
        val += A.get_value(row, col) * x[col];
    if (!is_eq_norm(val, b[row], 1e-4, 1e-5)) {
      Debug(0) << "Row " << row << " failed equality test ("
               << "should be: " << b[row] << ", is " << val << ")\n";
      feasible = false;
    }
  }
  return feasible;
}

std::ostream &operator<<(std::ostream &os, const LP::Column &c) {
  static std::unordered_map<ColType, std::string> ctos = {
      {ColType::Fixed, "Fixed"},
      {ColType::Bounded, "Bounded"},
      {ColType::LowerBound, "LowerBound"},
      {ColType::Free, "Free"},
      {ColType::UpperBound, "UpperBound"}};
  os << ctos[c.type] << ": " << c.lower << " <= x <= " << c.upper;
  return os;
}

} // namespace fizplex
