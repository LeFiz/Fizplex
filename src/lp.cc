#include "lp.h"
#include <cassert>

LP::Column::Column(ColType _type, double _lower, double _upper,
                   bool _is_logical)
    : type(_type), is_logical(_is_logical), lower(_lower), upper(_upper) {}

void LP::add_column(ColType _type, double _lower, double _upper,
                    bool _is_logical) {
  cols.push_back(Column(_type, _lower, _upper, _is_logical));
}

LP::Row::Row(RowType _type, double _lower, double _upper)
    : type(_type), lower(_lower), upper(_upper) {}

void LP::add_row(RowType _type, double _lower, double _upper) {
  rows.push_back(Row(_type, _lower, _upper));
}

size_t LP::column_count() const { return cols.size(); }

size_t LP::row_count() const { return rows.size(); }

void LP::add_value(size_t row, size_t column, double value) {
  assert(row < row_count());
  assert(column < column_count());

  cols[column].vec.add_value(row, value);
  rows[row].vec.add_value(column, value);
}

double LP::get_value(size_t row, size_t column) {
  assert(row < row_count());
  assert(column < column_count());

  auto d = cols[column].vec.get_value(row);
  assert(is_eq(d, rows[row].vec.get_value(column)));
  return d;
}

void LP::set_b() {
  for (const auto &row : rows) {
    if (row.type == RowType::LE)
      b.push_back(row.lower);
    else
      b.push_back(row.upper);
  }
}

void LP::add_logicals() { // WIP
  const size_t n = column_count();
  for (size_t i = 0; i < row_count(); i++) {
    Column col(ColType::Fixed, neg_inf, inf, true);
    switch (rows[i].type) {
    case RowType::Equality:
      col.type = ColType::Fixed;
      col.lower = 0.0;
      col.upper = 0.0;
      break;
    default:
      break;
    }
    cols.push_back(col);
    add_value(i, i + n, 1.0);
  }
}
