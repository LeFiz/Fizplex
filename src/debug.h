#include <iostream>

class Debug {
public:
  explicit Debug(std::ostream &_out = std::cerr) : out(_out){};
  template <typename T> Debug &operator<<(T t) {
    out << t;
    return *this;
  }

private:
  std::ostream &out;
};
