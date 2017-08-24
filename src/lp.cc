#include "lp.h"

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

size_t LP::column_count() { return cols.size(); }

size_t LP::row_count() { return rows.size(); }
