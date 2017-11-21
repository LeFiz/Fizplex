#include <iostream>

class Debug {
public:
  explicit Debug(int level, std::ostream &_out = std::cerr)
      : out(_out), print(level <= max_level){};

  template <typename T> Debug &operator<<(T t) {
    if (print)
      out << t;
    return *this;
  }
  static int get_max_level() { return max_level; }

private:
  std::ostream &out;
  static constexpr int max_level = 2;
  bool print;
};
