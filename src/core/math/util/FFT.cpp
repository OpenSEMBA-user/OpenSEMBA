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

#ifdef FFTW3_SUPPORT

#include "math/util/FFT.h"

namespace SEMBA {
namespace Math {
namespace Util {

std::vector<std::pair<Real,std::complex<Real>>> getFFT(
        const std::vector<std::pair<Real,std::complex<Real>>>& data,
        const bool inverse) {
    const size_t N = data.size();
    fftw_complex *in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    const int dir = (inverse)? FFTW_BACKWARD : FFTW_FORWARD;
    fftw_plan p = fftw_plan_dft_1d(N, in, out, dir, FFTW_ESTIMATE);

    for (size_t i = 0; i < N; ++i) {
        std::complex<Real> dataPoint = data[i].second;
        in[i][0] = dataPoint.real();
        in[i][1] = dataPoint.imag();
    }

    fftw_execute(p);

    Real timeStepSum = 0.0;
    for (size_t i = 0; i < N-1; ++i) {
        timeStepSum += data[i+1].first - data[i].first;
    }
    Real averageSamplingPeriod = (N > 1)? (timeStepSum/(N-1)) : (1);

    std::vector<std::pair<Real,std::complex<Real>>> res(N);
    for (size_t i = 0; i < N; ++i) {
        res[i].first = ((Real) i) / averageSamplingPeriod;

        std::complex<Real> dataPoint(out[i][0], out[i][1]);
        Real L = data.back().first - data.front().first;
        res[i].second = dataPoint * L / (Real) N ;
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

#endif
