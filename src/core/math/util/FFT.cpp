

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
