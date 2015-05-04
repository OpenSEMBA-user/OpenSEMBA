#include "gtest/gtest.h"
#include "ProjectFile.h"

class ProjectFileTest : public ::testing::Test {

};

TEST_F(ProjectFileTest, BasicOperations) {
    {
        ProjectFile lhs("./path/to/file");
        EXPECT_EQ(lhs, lhs);
        EXPECT_FALSE(lhs.canOpen());
        EXPECT_FALSE(lhs.canExecute());
    }
    {
        ProjectFile lhs("dir");
        EXPECT_TRUE(lhs.canExecute());
    }
}

TEST_F(ProjectFileTest, RelativePaths) {
    ProjectFile lhs("./path/to/file");
    ProjectFile rhs("./path/");
    EXPECT_EQ(lhs.relativeTo(rhs), ProjectFile("to/file"));
    EXPECT_EQ(lhs.relativeTo(lhs), ProjectFile("./"));
}
