#include "dvector.h"

class RhsScaler {
public:
  explicit RhsScaler(double);
  void scale(DVector &) const;
  void scale(double &) const;
  void unscale(DVector &) const;
  void unscale(double &) const;

private:
  double scale_factor = 0.0;
};
