#ifndef LP_H
#define LP_H

#include "colmatrix.h"
#include "dvector.h"
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
                  double obj_value = 0.0f);
  void add_row(RowType type, double lower, double upper);
  void add_row(RowType type, double lower, double upper, DVector row);
  void add_row(Row);
  void update_row_header(size_t ind, RowType type, double lower, double upper);
  size_t column_count() const;
  size_t row_count() const;
  void add_value(size_t row, size_t column, double value);
  double get_value(size_t row, size_t column) const;
  const LP::Column &column_header(size_t column) const;
  LP::Column &column_header(size_t column);
  const LP::Row &row_header(size_t row) const;

  void add_logicals();
  void add_obj_value(size_t ind, double d);
  double get_obj_value(size_t ind) const;

  bool is_feasible(const DVector &vec) const;
  ColMatrix A;
  DVector b;
  DVector c;

private:
  std::vector<Column> cols;
  std::vector<Row> rows;

  void set_b(size_t);
  void add_logical_column(ColType _type, double _lower, double _upper);
  void add_column(ColType type, double lower, double upper, double obj_value,
                  bool is_logical);
};

std::ostream &operator<<(std::ostream &os, const LP::Column &);

#endif
