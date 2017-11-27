#include "rowscaler.h"

namespace fizplex {

RowScaler::RowScaler(LP &_lp) : lp(_lp) {}

void RowScaler::scale_all() {
  for (size_t row = 0; row < lp.row_count(); row++) {
    double max = 0.0;
    for (size_t col = 0; col < lp.column_count(); col++) {
      if (max < std::fabs(lp.get_value(row, col))) {
        max = std::fabs(lp.get_value(row, col));
      }
    }
    if (max > 0.0) {
      for (size_t col = 0; col < lp.column_count(); col++) {
        if (std::fabs(lp.get_value(row, col)) > 0.0)
          lp.A.column(col)[row] /= max;
      }
      lp.b[row] /= max;
    }
  }
}

} // namespace fizplex
