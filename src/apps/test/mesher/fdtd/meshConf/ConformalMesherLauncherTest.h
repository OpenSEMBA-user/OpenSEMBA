#ifndef CONFORMALMESHERLAUNCHERTEST_H_
#define CONFORMALMESHERLAUNCHERTEST_H_

#include "gtest/gtest.h"
#include "parser/stl/ParserSTL.h"

class ConformalMesherLauncherTest : public ::testing::Test {

protected:

    ConformalMesherLauncherTest() {
    }

    virtual ~ConformalMesherLauncherTest() {
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

    void runUGRMesher(const string args) const {
        // TODO runUgrMesher
    }
};

#endif
