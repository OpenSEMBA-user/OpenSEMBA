#ifndef PARSERSTLTEST_H_
#define PARSERGIDTEST_H_

#include "gtest/gtest.h"
#include "parser/stl/ParserSTL.h"

class ParserSTLTest :
        public ::testing::Test,
        public ::testing::WithParamInterface<const char*> {

protected:

   ParserSTLTest() {
   }

   virtual ~ParserSTLTest() {
   }

   SmbData* parseFromSTL(const string project) {
       cout << "STL: " << project << endl;
       const string stlFolder("./projects/test/stls/");
       ParserSTL parser(stlFolder + project + ".stl");
       EXPECT_TRUE(parser.canOpen());
       SmbData* res = parser.read();
       EXPECT_TRUE(res != NULL);
       if (res != NULL) {
           EXPECT_TRUE(res->check());
       }
       return res;
   }

};

#endif
