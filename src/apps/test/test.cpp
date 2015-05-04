#include <stdio.h>
#include <gtest/gtest.h>
#include <string>

using namespace std;

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  string tests;

  tests += "*ProjectFile*:";
  tests += "*Math*:";
  tests += "*Geometry*:";
  tests += "*EMSource*:";
  tests += "*AdapterFDTDTest.OpenFOAMConversion*:";
  tests += "*AdapterFDTDTest.ugrMesherConversion*:";

  ::testing::GTEST_FLAG(filter) = tests.c_str();

  return RUN_ALL_TESTS();
}
