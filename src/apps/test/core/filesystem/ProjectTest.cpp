// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
    #warning "Filesystem::Project tests are not implemented for WIN32."
#endif
