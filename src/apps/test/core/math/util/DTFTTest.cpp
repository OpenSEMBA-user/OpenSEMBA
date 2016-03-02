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
#include <math/util/DTFT.h>
#include "gtest/gtest.h"

#include "math/vector/Cartesian.h"
#include "math/function/Gaussian.h"
#include "math/util/SpaceGenerator.h"

using namespace std;

using namespace SEMBA;
using namespace Math;


TEST(MathDTFTTest, gaussian) {
    const size_t nPoints = 10000;
    vector<Real> time = Util::linspace(pair<Real,Real>(0,100), nPoints);
    vector<pair<Real,complex<Real>>> input(nPoints);
    Function::Gaussian gaussian((Real) 0.5, (Real) 5.0);
    for (size_t i = 0; i < nPoints; ++i) {
        complex<Real> dataPoint(gaussian(time[i]), (Real) 0.0);
        input[i] = pair<Real,complex<Real>>(time[i], dataPoint);
    }

    {
        const Real fq = 0.0;
        EXPECT_NEAR(gaussian.getFourier(fq).real(), Util::getDTFT(input, fq).real(), 1e-2);
        EXPECT_NEAR(gaussian.getFourier(fq).imag(), Util::getDTFT(input, fq).imag(), 1e-2);
    }

}

