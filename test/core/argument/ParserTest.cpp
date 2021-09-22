

#include "ParserTest.h"

using namespace SEMBA;

TEST_F(ArgumentParserTest, basicOperation) {
    Argument::Parser args;
    // Loads available options.
    Argument::Switch* helpSwitch;
    EXPECT_NO_THROW(
            helpSwitch = (new Argument::Switch("Help", 'h', "help"))->defaultVal(false));
    args.addOption(helpSwitch);
    // Parses.
    Argument::Object res;
    EXPECT_NO_THROW(res = args.parse({"EXEC", "-h"}));
}
