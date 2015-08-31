#include "gtest/gtest.h"

#include "../../../../common/sources/PlaneWave.h"

class EMSourcePlaneWaveTest : public ::testing::Test {
    void SetUp() {

    }

    void TearDown() {

    }

protected:

};

TEST_F(EMSourcePlaneWaveTest, PolarCoordinatesDirAndPolarization) {
    {
        CVecR3 dir(1.0, 0.0, 0.0);
        CVecR3 pol(0.0, 0.0, 1.0);
        PlaneWave pw(NULL, ElemRGroup(), dir, pol);
        EXPECT_NEAR(1.5708, pw.getTheta(), 1e-3);
        EXPECT_NEAR(   0.0, pw.getPhi(),   1e-3);
        EXPECT_NEAR(   0.0, pw.getAlpha(), 1e-3);
        EXPECT_NEAR(   0.0, pw.getBeta(),  1e-3);
    }
}

