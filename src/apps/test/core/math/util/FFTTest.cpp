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

#include "math/function/Gaussian.h"
#include "math/util/FFT.h"
#include "math/util/SpaceGenerator.h"

using namespace std;

using namespace SEMBA;
using namespace Math;

#ifdef FFTW3_SUPPORT

TEST(MathFFTTest, gaussian) {
    const size_t nPoints = 10000;
    vector<Real> time = Util::linspace(pair<Real,Real>(-10,10), nPoints);
    vector<pair<Real,complex<Real>>> input(nPoints);
    Function::Gaussian gaussian((Real) 0.25, (Real) 0.0);
    for (size_t i = 0; i < nPoints; ++i) {
        complex<Real> dataPoint(gaussian(time[i]), (Real) 0.0);
        input[i] = pair<Real,complex<Real>>(time[i], dataPoint);
    }

    vector<pair<Real,complex<Real>>> out;
    EXPECT_NO_THROW(out = Util::getFFT(input));
    {
        complex<Real> outData = out[0].second;
        const Real fq = 0.0;
        EXPECT_EQ(gaussian.getFourier(fq).real(), outData.real());
        EXPECT_EQ(gaussian.getFourier(fq).imag(), outData.imag());
        EXPECT_EQ(fq, out[0].first);
    }

    vector<pair<Real,complex<Real>>> back;
    EXPECT_NO_THROW(back = Util::getFFT(out, true));
    const size_t testPoints = 3;
    for (size_t i = 0; i < testPoints; ++i) {
        complex<Real> inData = input[i].second;
        complex<Real> backData = back[i].second;
        EXPECT_EQ(inData.real(), backData.real());
        EXPECT_EQ(inData.imag(), backData.imag());
        EXPECT_EQ(input[i].first, back[i].first);
    }
}

#endif
