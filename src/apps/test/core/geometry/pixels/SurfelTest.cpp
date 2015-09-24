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
#include "geometry/pixels/Surfel.h"

class GeometrySurfelTest : public ::testing::Test {
};

TEST_F(GeometrySurfelTest, IsInto) {
    CVecI3 pos(1);
    Surfel surf(pos, CartesianDirection::dirX);

    CVecI3Frac sidePos(pos + CVecI3(1,0,0), CVecR3(0.0, 0.5, 0.0));
    EXPECT_TRUE(surf.isInto(sidePos)) <<
            "CVecI3Frac " << sidePos << " should be into surfel " << surf;
}

TEST_F(GeometrySurfelTest, Comparison) {
    Surfel surf1(CVecI3(1), CartesianDirection::dirX);
    Surfel surf2(CVecI3(1), CartesianDirection::dirY);
    EXPECT_LE(surf1, surf2);
    EXPECT_LE(surf1, Surfel(CVecI3(2), CartesianDirection::dirX));
    EXPECT_FALSE(surf1 < surf1);
}

TEST_F(GeometrySurfelTest, GetLinels) {
    CVecI3 pos(1);
    {
        Surfel surf(pos, CartesianDirection::dirX);
        SurfelLinels linels = surf.getLinels();
        EXPECT_EQ(Linel(pos, CartesianDirection::dirY), linels[0]);
        EXPECT_EQ(Linel(pos, CartesianDirection::dirZ), linels[1]);
        EXPECT_EQ(Linel(pos+CVecI3(0,0,1), CartesianDirection::dirY), linels[2]);
        EXPECT_EQ(Linel(pos+CVecI3(0,1,0), CartesianDirection::dirZ), linels[3]);
    }

    {
        Surfel surf(pos, CartesianDirection::dirY);
        SurfelLinels linels = surf.getLinels();
        EXPECT_EQ(Linel(pos, CartesianDirection::dirZ), linels[0]);
        EXPECT_EQ(Linel(pos, CartesianDirection::dirX), linels[1]);
        EXPECT_EQ(Linel(pos+CVecI3(1,0,0), CartesianDirection::dirZ), linels[2]);
        EXPECT_EQ(Linel(pos+CVecI3(0,0,1), CartesianDirection::dirX), linels[3]);
    }

    {
        Surfel surf(pos, CartesianDirection::dirZ);
        SurfelLinels linels = surf.getLinels();
        EXPECT_EQ(Linel(pos, CartesianDirection::dirX), linels[0]);
        EXPECT_EQ(Linel(pos, CartesianDirection::dirY), linels[1]);
        EXPECT_EQ(Linel(pos+CVecI3(0,1,0), CartesianDirection::dirX), linels[2]);
        EXPECT_EQ(Linel(pos+CVecI3(1,0,0), CartesianDirection::dirY), linels[3]);
    }
}
