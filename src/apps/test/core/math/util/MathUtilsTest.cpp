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

TEST(MathUtilsTest, equal) {
    std::vector<Real> num;
    num.push_back((Real) -2.69386e-10);
    num.push_back((Real) 1e-10);

    for (UInt i = 0; i < num.size(); i++) {
        EXPECT_TRUE(Util::equal(num[i], num[i]));
        EXPECT_TRUE(Util::equal(num[i], num[i], 0.0, 0.25));
    }

    EXPECT_FALSE(Util::equal(0.0, 0.17, 0.5, 0.01));
}
