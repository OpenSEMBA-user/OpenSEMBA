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

#ifdef EIGEN_SUPPORT

#include "gtest/gtest.h"
#include "math/fitting/VectorFitting.h"
#include "math/util/SpaceGenerator.h"

using namespace SEMBA;
using namespace Math;
using namespace Fitting;

using namespace std;

class MathFittingVectorFittingTest : public ::testing::Test {

};

TEST_F(MathFittingVectorFittingTest, ex1) {
    const size_t nS = 101;
    vector<Real> sImag = Util::logspace(pair<Real,Real>(0.0,4.0), nS);
    vector<VectorFitting::Sample> samples(nS);
    for (size_t i = 0; i < nS; i++) {
        samples[i].first =
                complex<Real>(0.0, - 2.0 * Constants::pi * sImag[i]);

        const complex<Real> sk = samples[i].first;
        vector<complex<Real>> f(1);
        f[0] =  2.0 /(sk+5.0)
                + complex<Real>(30.0,40.0) / (sk-(complex<Real>(-100.0,500.0)))
                + complex<Real>(30.0,-40.0)/ (sk-(complex<Real>(-100.0,-500.0)))
                + 0.5;
        samples[i].second = f;
    }

    VectorFitting fitting(samples);
    fitting.fit();

    EXPECT_NEAR(0.0, fitting.getRMS(), 1e-3);
}
#endif
