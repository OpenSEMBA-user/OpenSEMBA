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
#include "math/vector/Cartesian.h"

using namespace SEMBA;
using namespace Math;
using namespace Vector;

TEST(MathCartesianVectorTest, Basic) {
    EXPECT_EQ(CVecR3(2.0, 3.0, 4.0), CVecR3(2.0, 3.0, 4.0));
    EXPECT_NE(CVecR3(2.0, 3.0, 4.0), CVecR3(1.0, 3.0, 4.0));
    EXPECT_EQ(sqrt(4.0 + 9.0 + 16.0), CVecR3(2.0, 3.0, 4.0).norm());
    EXPECT_EQ(CVecR3(0.0, 0.0, 1.0), CVecR3(1.0,0.0,0.0) ^ CVecR3(0.0,1.0,0.0));
}

TEST(MathCartesianVectorTest, InitializerList) {
    CVecR3 a = {1.0, 2.0, 3.0};
    EXPECT_EQ(a, a);
    EXPECT_EQ(sqrt(3.0), a.norm());
}

TEST(MathCartesianVectorTest, Complex) {
    std::complex<double> zero(0.0, 0.0);
    std::complex<double> realOne(1.0, 0.0);
    CVecC3 a(realOne, realOne, realOne);
    EXPECT_EQ(a, a);
    EXPECT_EQ(sqrt(3.0), a.norm());
}

TEST(MathCartesianVectorTest, BinaryConversion) {
    for (UInt i = 0; i < 8; i++) {
        CVecI3 result((i/4)%2, (i/2)%2, i%2);
        CVecI3 converted;
        EXPECT_EQ(result, converted.setAsBinary(i));
    }
}
