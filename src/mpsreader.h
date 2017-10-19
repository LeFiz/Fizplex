#include "lp.h"
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std::string_view_literals;

class MPSReader {
public:
  MPSReader(std::string);
  LP read_lp();

private:
  using index_map = std::unordered_map<std::string, size_t>;
  LP lp;
  std::string obj_name;
  std::string mps_file;
  static constexpr int indicator_count = 4;
  static constexpr std::string_view indicators[indicator_count] = {
      "ROWS"sv, "COLUMNS"sv, "RHS"sv, "ENDATA"sv};

  std::string next_word(std::istringstream &);
  bool is_indicator(const std::string &) const;
  LP::Row parse_row_type(const std::string &word);
  void parse_and_add_rhs(const std::string &first_word, std::istringstream &,
                         const index_map &);
};
