
#include "gtest/gtest.h"

#include "source/PlaneWave.h"

using namespace SEMBA;
using namespace Source;
using namespace Geometry;

class EMSourcePlaneWaveTest : public ::testing::Test {
    void SetUp() {

    }

    void TearDown() {

    }

protected:

};

TEST_F(EMSourcePlaneWaveTest, PolarCoordinatesDirAndPolarization) {
    {
        Math::CVecR3 dir(1.0, 0.0, 0.0);
        Math::CVecR3 pol(0.0, 0.0, 1.0);
        PlaneWave pw(nullptr, ElemRGroup(), dir, pol);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getTheta(), 1e-3);
        EXPECT_NEAR(0.0,                   pw.getPhi(),   1e-3);
        EXPECT_NEAR(0.0,                   pw.getAlpha(), 1e-3);
        EXPECT_NEAR(0.0,                   pw.getBeta(),  1e-3);
    }

    {
        Math::CVecR3 dir(-1.0, 0.0, 0.0);
        Math::CVecR3 pol( 0.0, 1.0, 0.0);
        PlaneWave pw(nullptr, ElemRGroup(), dir, pol);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getTheta(), 1e-3);
        EXPECT_NEAR(Math::Constants::pi,   pw.getPhi(),   1e-3);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getAlpha(), 1e-3);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getBeta(),  1e-3);
    }

    {
        Math::CVecR3 dir(1.0, 0.0, 0.0);
        Math::CVecR3 pol(0.0, 1.0, 0.0);
        PlaneWave pw(nullptr, ElemRGroup(), dir, pol);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getTheta(), 1e-3);
        EXPECT_NEAR(0.0,                   pw.getPhi(),   1e-3);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getAlpha(), 1e-3);
        EXPECT_NEAR(Math::Constants::pi_2, pw.getBeta(),  1e-3);
    }

    {
        Math::CVecR3 dir(0.0, 0.0, -1.0);
        Math::CVecR3 pol(1.0, 1.0,  0.0);
        PlaneWave pw(nullptr, ElemRGroup(), dir, pol);
        EXPECT_NEAR(Math::Constants::pi,     pw.getTheta(), 1e-3);
        EXPECT_NEAR(0.0,                     pw.getPhi(),   1e-3);
        EXPECT_NEAR(Math::Constants::pi_2,   pw.getAlpha(), 1e-3);
        EXPECT_NEAR(Math::Constants::pi/4.0, pw.getBeta(),  1e-3);
    }

    {
        Math::CVecR3 dir(0.0, 0.0, -1.0);
        Math::CVecR3 pol(1.0, 1.0,  0.0);
        PlaneWave pw(nullptr, ElemRGroup(), dir, pol);
        EXPECT_NEAR(Math::Constants::pi,     pw.getTheta(), 1e-3);
        EXPECT_NEAR(0.0,                     pw.getPhi(),   1e-3);
        EXPECT_NEAR(Math::Constants::pi_2,   pw.getAlpha(), 1e-3);
        EXPECT_NEAR(Math::Constants::pi/4.0, pw.getBeta(),  1e-3);
    }

    {
        Math::CVecR3 dir( 0.0, 0.0, -1.0);
        Math::CVecR3 pol(-1.0, 1.0,  0.0);
        PlaneWave pw(nullptr, ElemRGroup(), dir, pol);
        EXPECT_NEAR(Math::Constants::pi,         pw.getTheta(), 1e-3);
        EXPECT_NEAR(0.0,                         pw.getPhi(),   1e-3);
        EXPECT_NEAR(Math::Constants::pi_2,       pw.getAlpha(), 1e-3);
        EXPECT_NEAR(Math::Constants::pi*3.0/4.0, pw.getBeta(),  1e-3);
    }
}

