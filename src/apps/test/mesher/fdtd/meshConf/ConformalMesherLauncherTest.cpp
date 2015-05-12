#include "ConformalMesherLauncherTest.h"

TEST_F(ConformalMesherLauncherTest,Structured){

    string args = "-auto 1 1 1 -s /home/luis/workspace/semba/projects/test/stls/B2.stl -o B2 --structured";
    runUGRMesher(args);

    SmbData* smb = parseFromSTL("B2");
    // TODO call conformal launcher.

    delete smb;
}

//INSTANTIATE_TEST_CASE_P(
//        stls, ConformalMesherLauncherTest, ::testing::Values(
//                "B2",
//                "ev55",
//                "scrub2"
//        ));
