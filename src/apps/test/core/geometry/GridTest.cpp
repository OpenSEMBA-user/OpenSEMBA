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

#include "GridTest.h"

using namespace SEMBA;
using namespace Geometry;
using namespace Math;
using namespace Constants;

TEST_F(GeometryGridTest, BasicOperations) {
    EXPECT_EQ(grid_.getNumCells(), CVecI3(20, 20, 20));
    EXPECT_EQ(Grid3(BoxR3(min_,min_), CVecR3()).getNumCells(), CVecI3(1,1,1));
    EXPECT_EQ(Grid3(BoxR3(min_,max_), CVecR3()).getNumCells(), CVecI3(1,1,1));
}

TEST_F(GeometryGridTest, NaturalCells) {
    const double tol = step_(CartesianAxis::x) / 1.0e3;
    EXPECT_EQ(grid_.getCell(min_, false, 0.0), CVecI3(0,0,0));
    EXPECT_EQ(grid_.getCell(max_, false, tol), grid_.getNumCells());
}

TEST_F(GeometryGridTest, NumberOfCells) {
    Grid3 grid(BoxR3(min_, max_), CVecR3(0.75, 0.75, 0.75));
    EXPECT_EQ(grid.getFullDomainBoundingBox(), BoxR3(min_,CVecR3(1.5,1.5,1.5)));
    EXPECT_EQ(grid.getNumCells(), CVecI3(2,2,2));
}

TEST_F(GeometryGridTest, PosInRange) {
    std::vector<Real> posInRange = grid_.getPosInRange(x,0.17,0.27);
    EXPECT_EQ(2, posInRange.size());
    EXPECT_EQ(0.2, posInRange[0]);
    EXPECT_EQ(0.25, posInRange[1]);
}

TEST_F(GeometryGridTest, EnlargeBound) {
    CVecR3 padding(0.6, 0.6, 0.6);
    CVecR3 cellSizes(0.2, 0.2, 0.2);
    std::pair<CVecR3,CVecR3> pad(padding, padding);
    std::pair<CVecR3,CVecR3> siz(cellSizes, cellSizes);
    CVecI3 numCellsBefore = grid_.getNumCells();
    grid_.enlarge(pad, siz);
    EXPECT_EQ(numCellsBefore + 6, grid_.getNumCells());
}

//TEST_F(GeometryGridTest, GetCVecI3Fractional) {
//    bool err;
//    EXPECT_EQ(CVecI3Frac(CVecI3(0), CVecR3(0.0)),
//               grid_.getCVecI3Fractional(CVecR3(0.0), err));
//    EXPECT_TRUE(err);
//
//    EXPECT_EQ(CVecI3Frac(CVecI3(3), CVecR3(0.02/0.05)),
//               grid_.getCVecI3Fractional(CVecR3(0.17), err));
//    EXPECT_TRUE(err);
//
//    EXPECT_EQ(CVecI3Frac(CVecI3(4), CVecR3(0.0)),
//               grid_.getCVecI3Fractional(CVecR3(0.2), err));
//    EXPECT_TRUE(err);
//
//    grid_.getCVecI3Fractional(CVecR3(1.01), err);
//    EXPECT_FALSE(err);
//
//    grid_.getCVecI3Fractional(CVecR3(-0.01), err);
//    EXPECT_FALSE(err);
//}

TEST_F(GeometryGridTest, GetSteps) {
    EXPECT_NEAR(0.05, grid_.getStep(0,2), Util::tolerance);
    EXPECT_NEAR(0.05, grid_.getStep(1,5), Util::tolerance);
    EXPECT_NEAR(0.05, grid_.getStep(2,5), Util::tolerance);
}
