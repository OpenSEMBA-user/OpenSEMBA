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
#include "geometry/pixels/Voxel.h"

class GeometryVoxelTest : public ::testing::Test {
};

TEST_F(GeometryVoxelTest, IsInto) {
    CVecI3 pos(1);
    Voxel local(pos);
    for (UInt i = 0; i < 8; i++) {
        CVecI3 offset((i/4)%2, (i/2)%2, i%2);
        CVecI3Frac infVertex(pos + offset, CVecR3(0.0));
        EXPECT_TRUE(local.isInto(infVertex))
                << "CVecI3Frac " << infVertex << " should be into voxel " << local;

        CVecI3Frac center(pos+offset, CVecR3(0.5));
        if (offset == CVecI3(0,0,0)) {
            EXPECT_TRUE(local.isInto(center))
                    << "CVecI3Frac " << center << " should be into voxel " << local;
        } else {
            EXPECT_FALSE(local.isInto(center))
                    << "CVecI3Frac " << center << " should not be into voxel " << local;
        }

        CVecI3Frac centerInfX(pos + offset, CVecR3(0.0, 0.5, 0.5));
        if (offset == CVecI3(0,0,0) || offset == CVecI3(1,0,0)) {
            EXPECT_TRUE(local.isInto(centerInfX))
                << "CVecI3Frac " << centerInfX << " should be into voxel " << local;
        } else {
            EXPECT_FALSE(local.isInto(centerInfX))
                << "CVecI3Frac " << centerInfX << " should not be into voxel " << local;
        }
    }
}

//TEST_F(GeometryVoxelTest, GetLinels) {
//    CVecI3 pos(1);
//    Voxel local(pos);
//
//    VoxelLinels linels = local.getLinels();
//    EXPECT_EQ(Linel(pos,                 CartesianDirection::dirY), linels[0]);
//    EXPECT_EQ(Linel(pos + CVecI3(0,0,1), CartesianDirection::dirY), linels[1]);
//    EXPECT_EQ(Linel(pos + CVecI3(1,0,0), CartesianDirection::dirY), linels[2]);
//    EXPECT_EQ(Linel(pos + CVecI3(1,0,1), CartesianDirection::dirY), linels[3]);
//    EXPECT_EQ(Linel(pos,                 CartesianDirection::dirZ), linels[4]);
//    EXPECT_EQ(Linel(pos + CVecI3(1,0,0), CartesianDirection::dirZ), linels[5]);
//    EXPECT_EQ(Linel(pos + CVecI3(0,1,0), CartesianDirection::dirZ), linels[6]);
//    EXPECT_EQ(Linel(pos + CVecI3(1,1,0), CartesianDirection::dirZ), linels[7]);
//    EXPECT_EQ(Linel(pos,                 CartesianDirection::dirX), linels[8]);
//    EXPECT_EQ(Linel(pos + CVecI3(0,1,0), CartesianDirection::dirX), linels[9]);
//    EXPECT_EQ(Linel(pos + CVecI3(0,0,1), CartesianDirection::dirX), linels[10]);
//    EXPECT_EQ(Linel(pos + CVecI3(0,1,1), CartesianDirection::dirX), linels[11]);
//}
//
//TEST_F(GeometryVoxelTest, GetSurfels) {
//    CVecI3 pos(1);
//    Voxel local(pos);
//
//    VoxelSurfels surfels = local.getSurfels();
//    EXPECT_EQ(Surfel(pos                , CartesianDirection::dirX), surfels[0]);
//    EXPECT_EQ(Surfel(pos + CVecI3(1,0,0), CartesianDirection::dirX), surfels[1]);
//    EXPECT_EQ(Surfel(pos                , CartesianDirection::dirY), surfels[2]);
//    EXPECT_EQ(Surfel(pos + CVecI3(0,1,0), CartesianDirection::dirY), surfels[3]);
//    EXPECT_EQ(Surfel(pos                , CartesianDirection::dirZ), surfels[4]);
//    EXPECT_EQ(Surfel(pos + CVecI3(0,0,1), CartesianDirection::dirZ), surfels[5]);
//}
