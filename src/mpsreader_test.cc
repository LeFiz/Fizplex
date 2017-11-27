#include "mpsreader.h"
#include "gtest/gtest.h"
#include <exception>

namespace fizplex {

TEST(MPSReaderTest, ReadSimpleMPS) {
  LP lp = MPSReader::read_lp("./test/simple.mps");
  EXPECT_EQ(lp.row_header(0u).type, RowType::LE);
  EXPECT_EQ(lp.row_header(1u).type, RowType::GE);
  EXPECT_EQ(lp.row_header(2u).type, RowType::Equality);
  EXPECT_EQ(lp.row_header(3u).type, RowType::NonBinding);

  EXPECT_EQ(4u, lp.row_count());

  EXPECT_DOUBLE_EQ(2.0, lp.row_header(0u).upper);
  EXPECT_DOUBLE_EQ(3.1, lp.row_header(1u).lower);
  EXPECT_DOUBLE_EQ(4.0, lp.row_header(2u).lower);
  EXPECT_DOUBLE_EQ(4.0, lp.row_header(2u).upper);

  EXPECT_EQ(1u, lp.column_count());
  EXPECT_DOUBLE_EQ(1.1, lp.get_value(0, 0));
  EXPECT_DOUBLE_EQ(-0.43, lp.get_value(2, 0));
  EXPECT_DOUBLE_EQ(1.5, lp.get_obj_value(0));
}

TEST(MPSReaderTest, MissingRowNameThrows) {
  EXPECT_THROW(MPSReader::read_lp("./test/missing_row_name.mps"),
               std::runtime_error);
}

TEST(MPSReaderTest, WrongRowTypeThrows) {
  EXPECT_THROW(MPSReader::read_lp("./test/wrong_row_type.mps"),
               std::runtime_error);
}

TEST(MPSReaderTest, NonExistingFileThrows) {
  EXPECT_THROW(MPSReader::read_lp("./test/non_existing_file.mps"),
               std::runtime_error);
}

TEST(MPSReaderTest, BadRhsNumberFormatThrows) {
  EXPECT_THROW(MPSReader::read_lp("./test/bad_rhs_number_format.mps"),
               std::invalid_argument);
}

TEST(MPSReaderTest, MissingRHSNameIsValid) {
  LP lp = MPSReader::read_lp("./test/missing_rhs_name.mps");
  EXPECT_DOUBLE_EQ(2.0, lp.row_header(0u).upper);
  EXPECT_DOUBLE_EQ(4.0, lp.row_header(2u).upper);
}

} // namespace fizplex
