#ifndef LP_H
#define LP_H

#include "numeric.h"
#include "svector.h"
#include <string>

enum class ColType { Fixed, Bounded, LowerBound, Free, UpperBound };
enum class RowType { Equality, Range, LE, Free, GE };

class LP {
public:
  struct Column {
    //    std::string id;
    ColType type;
    bool is_logical;
    double lower;
    double upper;
    SVector vec;
    Column(ColType type, double lower, double upper, bool is_logical = false);
  };

  struct Row {
    //    std::string id;
    RowType type;
    double lower;
    double upper;
    SVector vec;
    Row(RowType type, double lower, double upper);
  };

  void add_column(ColType type, double lower, double upper,
                  bool is_logical = false);
  void add_row(RowType type, double lower, double upper);
  size_t column_count();
  size_t row_count();

private:
  std::vector<Column> cols;
  std::vector<Row> rows;
};

#endif
