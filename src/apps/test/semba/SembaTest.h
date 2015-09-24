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
//#ifndef SEMBATEST_H_
//#define SEMBATEST_H_
//
//#include "gtest/gtest.h"
//#include "ProjectFile.h"
//#include "options/Arguments.h"
//#include "semba/Semba.h"
//
//class SembaTest : public ::testing::Test {
//
//protected:
//
//   ProjectFile getProjectFile(const string project) {
//      const string testFolder("./projects/test/");
//      ProjectFile res(testFolder + project + ".gid/" + project + ".smb");
//      EXPECT_TRUE(res.canOpen());
//      return res;
//   }
//
//   void runProject(const string& projectName, const string& inputs = "") {
//       ProjectFile project = getProjectFile(projectName);
//       Arguments args("-i " + project.getFilename() + inputs);
//       Semba semba(args);
//       EXPECT_TRUE(semba.canRun());
//       if (semba.canRun()) {
//           semba.run();
//       }
//   }
//
//};
//
//#endif
