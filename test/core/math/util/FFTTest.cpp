
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
    const size_t nPoints = 100000;
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
        EXPECT_NEAR(abs(gaussian.getFourier(fq)), abs(outData),1e-2);
        EXPECT_NEAR(arg(gaussian.getFourier(fq)), arg(outData),1e-2);
        EXPECT_EQ(fq, out[0].first);
    }

    vector<pair<Real,complex<Real>>> back;
    EXPECT_NO_THROW(back = Util::getFFT(out, true));
    const size_t testPoints = 3;
    for (size_t i = 0; i < testPoints; ++i) {
        complex<Real> inData = input[i].second;
        complex<Real> backData = back[i].second;
        EXPECT_NEAR(abs(inData), abs(backData), 1e-2);
    }
}

#endif
