#include "gtest/gtest.h"
#include "options/Arguments.h"
#include "options/OptionsSolver.h"

class OptionsSolverTest :
        public ::testing::Test,
        public ::testing::WithParamInterface<const char*> {

};

TEST_P(OptionsSolverTest, conversion){
    string str = GetParam();
    string pre = "EXEC -i ./projects/test/sphere.gid/sphere.smb";
    Arguments inputArg(pre + str);
    OptionsSolver options;
    options.set(inputArg);
    Arguments convertedArg(pre + options.toArgsStr());
    EXPECT_TRUE(convertedArg.contains(inputArg));
}

INSTANTIATE_TEST_CASE_P(
      goodArguments,
      OptionsSolverTest,
      ::testing::Values(
            " -mtln -groundwires",
            " -attw 0.1 -pmlalpha 0.2 0.1",
            " -ade -taparrabos",
            " -pmlcorr 0.1 14",
            " -inductance ledfelt"
            ));
