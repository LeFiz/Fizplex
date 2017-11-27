#include "lp.h"
#include <string>
#include <string_view>
#include <unordered_map>

namespace fizplex {
namespace MPSReader {

using namespace std::string_view_literals;
using index_map = std::unordered_map<std::string, size_t>;

LP read_lp(std::string file);

constexpr int indicator_count = 4;
constexpr std::string_view indicators[indicator_count] = {"ROWS"sv, "COLUMNS"sv,
                                                          "RHS"sv, "ENDATA"sv};

std::string next_word(std::istringstream &);
bool is_indicator(const std::string &);
LP::Row parse_row_type(const std::string &word);
void parse_and_add_rhs(const std::string &first_word, std::istringstream &,
                       const index_map &, LP &);
} // namespace MPSReader
} // namespace fizplex
