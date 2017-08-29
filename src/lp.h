#ifndef LP_H
#define LP_H

#include "colmatrix.h"
#include "svector.h"
#include <valarray>

enum class ColType { Fixed, Bounded, LowerBound, Free, UpperBound };
enum class RowType { Equality, Range, LE, NonBinding, GE };

class LP {
public:
  struct Column {
    ColType type;
    bool is_logical;
    double lower;
    double upper;
    Column(ColType type, double lower, double upper, bool is_logical = false);
  };

  struct Row {
    RowType type;
    double lower;
    double upper;
    Row(RowType type, double lower, double upper);
  };

  LP();

  void add_column(ColType type, double lower, double upper,
                  bool is_logical = false);
  void add_row(RowType type, double lower, double upper);
  size_t column_count() const;
  size_t row_count() const;
  void add_value(size_t row, size_t column, double value);
  double get_value(size_t row, size_t column);
  const LP::Column &column_header(size_t column) const;

  void add_logicals();
  void set_b();
  void add_obj_value(size_t ind, double d);
  double get_obj_value(size_t ind) const;

  ColMatrix A;
  std::valarray<double> b;
  std::valarray<double> c;

private:
  std::vector<Column> cols;
  std::vector<Row> rows;
};

#endif
