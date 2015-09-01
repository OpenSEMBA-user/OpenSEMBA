#ifndef PARSERSTLTEST_H_
#define PARSERGIDTEST_H_

#include "gtest/gtest.h"
#include "parser/stl/ParserSTL.h"
#include "exporter/vtk/ExporterVTK.h"

class ParserSTLTest :
public ::testing::Test,
public ::testing::WithParamInterface<const char*> {

    void SetUp() {
//        stlFolder_ = "./projects/test/stls/";
    }

protected:

    ParserSTLTest() {
        stlFolder_ = "./projects/test/stls/";
    }

    virtual ~ParserSTLTest() {
    }

    string stlFolder_;

    SmbData* parseFromSTL(const string project) const {
        cout << "STL: " << project << endl;
        ParserSTL parser(stlFolder_ + project + ".stl");
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
