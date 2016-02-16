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
#include "math/axis/Local.h"
#include "math/Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class MathLocalAxesTest : public ::testing::Test {

};

TEST_F(MathLocalAxesTest, BasicIdentities) {

    SEMBA::Math::Axis global(CVecR3(0.0));

    CVecR3 localOrigin(3.0,0.0,0.0);
    LocalAxes local(CVecR3(Constants::pi, Constants::pi_2, Constants::pi / 3.0), localOrigin);

    MatR33 eye;
    eye.eye();

    EXPECT_EQ(eye, global.getTransformationMatrix());

    MatR33 localTransf = local.getTransformationMatrix();
    MatR33 localTransfTransposed = local.getTransformationMatrix().transpose();
    EXPECT_EQ(eye, localTransf * localTransfTransposed);

    EXPECT_EQ(localOrigin, local.convertToGlobal(CVecR3(0.0)));
}

TEST_F(MathLocalAxesTest, VectorTransformation) {
    LocalAxes global(CVecR3(0.0));
    LocalAxes local(CVecR3(Constants::pi/2, 0.0, 0.0));

    CVecR3 localV(1.0,1.0,0.0);
    EXPECT_EQ(CVecR3(-1.0,1.0,1.0), local.convertToGlobal(localV));
}

TEST_F(MathLocalAxesTest, TensorTransformation) {
    LocalAxes global(CVecR3(0.0));

    const Int rotations = 3;
    LocalAxes local(CVecR3(Constants::pi*2.0/((Real)rotations), 0.0, 0.0));

    MatR33 original;

    original(0,0) = 1;
    original(0,1) = 2;
    original(0,2) = 3;

    original(1,0) = 5;
    original(1,1) = 7;
    original(1,2) = 11;

    original(2,0) = 13;
    original(2,1) = 17;
    original(2,2) = 19;

    MatR33 rotated = original;
    for (Int i = 0; i < rotations; i++) {
        rotated = local.convertToGlobal(rotated);
    }
    EXPECT_EQ(original, rotated);
}
