#include "debug.h"
#include "gtest/gtest.h"

TEST(DebugTest, WriteToStringStream) {
  std::ostringstream oss;
  const std::string str = "Hello Debug!";
  Debug(oss) << str;
  EXPECT_EQ(str, oss.str());
}
