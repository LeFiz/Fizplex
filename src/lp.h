#ifndef LP_H
#define LP_H

#include "svector.h"
#include <valarray>

enum class ColType { Fixed, Bounded, LowerBound, Free, UpperBound };
enum class RowType { Equality, Range, LE, Free, GE };

class LP {
public:
  struct Column {
    ColType type;
    bool is_logical;
    double lower;
    double upper;
    SVector vec;
    Column(ColType type, double lower, double upper, bool is_logical = false);
  };

  struct Row {
    RowType type;
    double lower;
    double upper;
    SVector vec;
    Row(RowType type, double lower, double upper);
  };

  void add_column(ColType type, double lower, double upper,
                  bool is_logical = false);
  void add_row(RowType type, double lower, double upper);
  size_t column_count() const;
  size_t row_count() const;
  void add_value(size_t row, size_t column, double value);
  double get_value(size_t row, size_t column);

  void add_logicals();

  void set_b();
  std::valarray<double> b;

private:
  std::vector<Column> cols;
  std::vector<Row> rows;
};

#endif
