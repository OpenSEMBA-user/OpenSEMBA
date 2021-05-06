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

