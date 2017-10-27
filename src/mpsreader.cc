#include "mpsreader.h"
#include <exception>
#include <fstream>

MPSReader::MPSReader(std::string mps_file_) : mps_file(mps_file_) {}

LP MPSReader::read_lp() {
  std::ifstream ifs(mps_file);
  if (!ifs.is_open())
    throw std::runtime_error("Couldn't open file " + mps_file);
  std::string line, current_section;
  index_map rows, vars;
  while (std::getline(ifs, line)) {
    std::istringstream iss(line);
    const auto first_word = next_word(iss);
    if (is_indicator(first_word) && current_section != first_word)
      current_section = first_word;
    else { // Data record
      if (current_section == "ROWS") {
        const std::string row_name = next_word(iss);
        if (first_word == "N" && obj_name.empty())
          obj_name = row_name;
        else {
          lp.add_row(parse_row_type(first_word));
          if (!row_name.empty())
            rows[row_name] = lp.row_count() - 1;
          else
            throw std::runtime_error("Row name is empty");
        }
      } else if (current_section == "RHS")
        parse_and_add_rhs(first_word, iss, rows);
      else if (current_section == "COLUMNS") {
        const std::string var_name = first_word;
        if (vars.find(var_name) == vars.end()) {
          lp.add_column(ColType::LowerBound, 0, inf);
          vars[var_name] = lp.column_count() - 1;
        }
        while (iss.rdbuf()->in_avail() > 0) {
          const std::string row_name = next_word(iss);
          const auto val = std::stod(next_word(iss));
          if (row_name == obj_name)
            lp.add_obj_value(vars[var_name], val);
          else
            lp.add_value(rows[row_name], vars[var_name], val);
        }
      }
    }
  }
  return lp;
}

std::string MPSReader::next_word(std::istringstream &iss) {
  std::string word;
  iss >> word;
  return word;
}

bool MPSReader::is_indicator(const std::string &word) const {
  for (int i = 0; i < indicator_count; ++i) {
    if (word == indicators[i])
      return true;
  }
  return false;
}

LP::Row MPSReader::parse_row_type(const std::string &word) {
  if (word == "L")
    return (LP::Row(RowType::LE, -inf, 0.0));
  else if (word == "G")
    return (LP::Row(RowType::GE, 0.0, inf));
  else if (word == "E")
    return (LP::Row(RowType::Equality, 0.0, 0.0));
  else if (word == "N")
    return (LP::Row(RowType::NonBinding, -inf, inf));
  else
    throw std::runtime_error("Could not parse \"" + word + "\" as Row type");
}

void MPSReader::parse_and_add_rhs(const std::string &first_word,
                                  std::istringstream &iss,
                                  const index_map &rows) {
  bool has_rhs_name = (rows.find(first_word) == rows.end());
  while (iss.rdbuf()->in_avail() > 0) {
    const auto row_name = has_rhs_name ? next_word(iss) : first_word;
    has_rhs_name = true;
    const auto rhs_value = std::stod(next_word(iss));
    const LP::Row &rh = lp.row_header(rows.at(row_name));
    switch (rh.type) {
    case RowType::LE:
      lp.update_row_header(rows.at(row_name), rh.type, rh.lower, rhs_value);
      break;
    case RowType::GE:
      lp.update_row_header(rows.at(row_name), rh.type, rhs_value, rh.upper);
      break;
    case RowType::Equality:
      lp.update_row_header(rows.at(row_name), rh.type, rhs_value, rhs_value);
      break;
    case RowType::NonBinding: // RHS irrelevant
      break;
    default:
      break;
    }
  }
}
