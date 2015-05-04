#include "gtest/gtest.h"
#include "ProjectFile.h"

#ifndef _WIN32
class ProjectFileTest : public ::testing::Test {
    void SetUp() {
        file_ = ProjectFile("/usr/bin/ls");
    }
protected:
    ProjectFile file_;
};

TEST_F(ProjectFileTest, BasicOperations) {
    EXPECT_EQ(file_, file_);
//    EXPECT_TRUE(file_.canOpen());
//    EXPECT_TRUE(file_.canExecute());
}

TEST_F(ProjectFileTest, FolderOperations) {
    EXPECT_EQ(file_.getFolder(), "/usr/bin/");
    ProjectFile folder(file_.getFolder());
    EXPECT_EQ(folder.getFolder(), "/");
}

TEST_F(ProjectFileTest, RelativePaths) {
    ProjectFile rhs("./path/");
    EXPECT_EQ(file_.relativeTo(rhs), ProjectFile("to/file"));
    EXPECT_EQ(file_.relativeTo(file_), ProjectFile("./"));
}
#else
    #warning "Project file tests are not implemented for WIN32."
#endif
