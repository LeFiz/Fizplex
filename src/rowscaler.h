#ifndef ROWSCALER_H
#define ROWSCALER_H

#include "lp.h"

class RowScaler {
public:
  explicit RowScaler(LP &);
  void scale_all();

private:
  LP &lp;
};

#endif
