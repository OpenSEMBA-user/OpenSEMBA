
#include "gtest/gtest.h"
#include "filesystem/Project.h"

using namespace SEMBA;
using namespace FileSystem;

#ifndef _WIN32
class ProjectTest : public ::testing::Test {
    void SetUp() {
        file_ = Project("/usr/bin/ls");
    }
protected:
    Project file_;
};

TEST_F(ProjectTest, basicOperations) {
    EXPECT_EQ(file_, file_);
}

TEST_F(ProjectTest, folderOperations) {
    EXPECT_TRUE(Project("/usr/bin/").isFolder());
    EXPECT_FALSE(Project("/non/existing/folder/").isFolder());
    EXPECT_EQ(file_.getFolder(), "/usr/bin/");
    Project usrFolder(file_.getFolder());
    EXPECT_EQ("/usr/", usrFolder.getFolder());
    Project rootFolder(usrFolder.getFolder());
    EXPECT_EQ("/", rootFolder.getFolder());
}

TEST_F(ProjectTest, relativePaths) {
    Project rhs("/usr/");
    EXPECT_EQ(file_.relativeTo(file_), Project("ls"));
    EXPECT_EQ(file_.relativeTo(rhs), Project("bin/ls"));
}

TEST_F(ProjectTest, createDirs) {
    char currentPath[FILENAME_MAX];
    getcwd(currentPath, sizeof(currentPath));
    Project original(currentPath);
    Project test(original + "/projectTestFolder/");

    EXPECT_NO_THROW(test.rmDir());
    EXPECT_NO_THROW(test.makeDir());
    EXPECT_NO_THROW(test.changeDir());

    getcwd(currentPath, sizeof(currentPath));
    EXPECT_EQ(std::string(currentPath)+"/", test) << "Actual: " << test;

    original.changeDir();
    getcwd(currentPath, sizeof(currentPath));
    EXPECT_EQ(std::string(currentPath), original) << "Actual: " << original;

    test.rmDir();
}

#else

TEST(ProjectTest, canHandleFileExtensions) {
    Project p;

    std::vector<std::pair<std::string, std::string>> list{
        std::make_pair("/my/file/extension.program.subprogram", ".program.subprogram"),
        std::make_pair("/my/file.gid/extension.program.subprogram", ".program.subprogram"),
        std::make_pair("extension.program.subprogram", ".program.subprogram"),
        std::make_pair("/my/file/extension.extension", ".extension"),
        std::make_pair("/my/file.gid/extension.extension", ".extension"),
        std::make_pair("extension.extension", ".extension"),
        // Backslashes
        std::make_pair("C:\\folder\\extension.program.subprogram", ".program.subprogram"),
        std::make_pair("C:\\folder.gid\\extension.program.subprogram", ".program.subprogram"),
        std::make_pair("extension.program.subprogram", ".program.subprogram"),
        std::make_pair("C:\\folder\\extension.extension", ".extension"),
        std::make_pair("C:\\folder.gid\\extension.extension", ".extension"),
        std::make_pair("extension.extension", ".extension"),
    };

    for (const auto& pair : list) {
        p.setFilename(pair.first);
        EXPECT_EQ(pair.second, p.getExtension());
    }
}

#endif
