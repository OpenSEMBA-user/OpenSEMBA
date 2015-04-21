#include "gtest/gtest.h"
#include "options/Arguments.h"

class ArgumentsTest :
        public ::testing::Test,
        public ::testing::WithParamInterface<const char*> {

};


TEST_P(ArgumentsTest, contains){
    string str = GetParam();
    str = "EXEC -i ./projects/test/sphere.gid/sphere.smb " + str;
    Arguments argLHS(str + " -ZZ 0 1 2");
    Arguments argRHS(str);
    EXPECT_TRUE(argLHS.contains(argRHS));
}

INSTANTIATE_TEST_CASE_P(
      goodArguments,
      ArgumentsTest,
      ::testing::Values(
            "-XX -YY",
            "-XX 0 1 2",
            "-abc  -cba"
            ));
