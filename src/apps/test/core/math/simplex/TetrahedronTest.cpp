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
#include "math/simplex/Tetrahedron.h"

using namespace SEMBA;
using namespace Math;

//TEST_F(MathMatrixSimplexTetrahedronTest, BasicOperations) {
//   Static<double,2,2> c;
//   // Addition
//   c(0,0) = 8.0; c(0,1) = 13.0;
//   c(1,0) = 16.0; c(1,1) = 22.0;
//   EXPECT_EQ(c, a_ + b_);
//   // Product
//   c(0,0) = 33.0; c(0,1) = 45.0;
//   c(1,0) = 86.0; c(1,1) = 118.0;
//   EXPECT_EQ(c, a_ * b_);
//   // Operator+=
//   c.zeros();
//   Static<double,2,2> def;
//   EXPECT_EQ(c, def);
//   c += a_;
//   EXPECT_EQ(c, a_);
//   EXPECT_EQ(c *= 2.0, a_ * 2.0);
//   // Transpose
//   EXPECT_EQ(a_, a_.transpose().transpose());
//}
