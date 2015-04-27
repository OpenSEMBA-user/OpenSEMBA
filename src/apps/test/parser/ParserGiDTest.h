#ifndef PARSERGIDTEST_H_
#define PARSERGIDTEST_H_

#include "gtest/gtest.h"
#include "parser/gid/ParserGiD.h"

class ParserGiDTest : public ::testing::Test {

protected:

   ParserGiDTest() {
   }

   virtual ~ParserGiDTest() {
   }

   SmbData*
   newSmb(const string project) {
      cout << "Project: " << project << endl;
      const string testFolder("./projects/test/");
      ParserGiD parser(testFolder+project+".gid/"+project+".smb");
      EXPECT_TRUE(parser.canOpen());
      SmbData* res = parser.read();
      EXPECT_TRUE(res != NULL);
      EXPECT_TRUE(res->check());
      return res;
   }
};

#endif
