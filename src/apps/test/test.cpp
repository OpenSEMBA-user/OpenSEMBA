#include <stdio.h>
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  ::testing::GTEST_FLAG(filter) = "*AdapterFDTD*";

  return RUN_ALL_TESTS();
}
