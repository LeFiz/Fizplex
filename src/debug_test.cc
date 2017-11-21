#include "debug.h"
#include "gtest/gtest.h"

class DebugTest : public ::testing::Test {
public:
  std::ostringstream oss;
  const std::string str = "Hello Debug!";
  const int max_debug_level = Debug::get_max_level();
};

TEST_F(DebugTest, AtMaxDebugLevelItWritesToOutStream) {
  Debug(max_debug_level, oss) << str;
  EXPECT_EQ(str, oss.str());
}

TEST_F(DebugTest, CanBeChained) {
  Debug(max_debug_level, oss) << str << str;
  EXPECT_EQ(str + str, oss.str());
}

TEST_F(DebugTest, AboveMaxLevelItDoesntWrite) {
  Debug(max_debug_level + 1, oss) << str;
  EXPECT_TRUE(oss.str().empty());
}
