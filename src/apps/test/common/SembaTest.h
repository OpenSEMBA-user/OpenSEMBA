#ifndef SEMBATEST_H_
#define SEMBATEST_H_

#include "gtest/gtest.h"
#include "ProjectFile.h"
#include "options/Arguments.h"
//#include "Semba.h"

class SembaTest : public ::testing::Test {

protected:

   ProjectFile getProjectFile(const string project) {
      const string testFolder("./projects/test/");
      ProjectFile res(testFolder + project + ".gid/" + project + ".smb");
      EXPECT_TRUE(res.canOpen());
      return res;
   }

   void runProject(const string& projectName, const string& inputs = "") {
       ProjectFile project = getProjectFile(projectName);
       Arguments args("-i " + project.getFilename() + inputs);
       Semba semba(args);
       EXPECT_TRUE(semba.canRun());
       if (semba.canRun()) {
           semba.run();
       }
   }

};

#endif
