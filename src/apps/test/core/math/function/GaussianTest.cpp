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

#include "math/function/Gaussian.h"

using namespace std;

using namespace SEMBA;
using namespace Math;
using namespace Function;

TEST(MathFunctionGaussianTest, fourier) {
    {
        const Real spread = 0.5;
        const Real delay  = 5;
        Gaussian gaussian(spread, delay);
        EXPECT_EQ(0.0, gaussian.getFourier(0.0).imag());
        EXPECT_EQ(spread*sqrt(2.0 * Constants::pi),
                gaussian.getFourier(0.0).real());
    }

}
