#include "ParserSTLTest.h"

TEST_P(ParserSTLTest, readsSomething) {
    string project = GetParam();
    SmbData* smb = parseFromSTL(project);
    EXPECT_TRUE(smb->mesh != NULL);
}

INSTANTIATE_TEST_CASE_P(
        stls, ParserSTLTest, ::testing::Values(
                "single",
                "B2",
                "ev55",
                "scrub2",
                "dmcwf"
        ));
