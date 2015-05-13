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

    void runUGRMesher(const string project, const string args) const {
        // TODO runUgrMesher
    }
};

#endif
