
#include <math/util/DTFT.h>
#include "gtest/gtest.h"

#include "math/vector/Cartesian.h"
#include "math/function/Gaussian.h"
#include "math/util/SpaceGenerator.h"

using namespace std;

using namespace SEMBA;
using namespace Math;

TEST(MathDTFTTest, dirac) {
    const size_t nPoints = 10000;
    vector<Real> time = Util::linspace(pair<Real,Real>(0,10), nPoints);
    vector<pair<Real,complex<Real>>> input(nPoints);
    for (size_t i = 0; i < nPoints; ++i) {
        complex<Real> dataPoint(0.0, 0.0);
        input[i] = pair<Real,complex<Real>>(time[i], dataPoint);
    }
    input[0].second = complex<Real>(1.0, 0.0);

    const size_t nFq = 3;
    vector<Real> fq = Util::linspace(pair<Real,Real>(0, 3), nFq);
    for (size_t i = 0; i < fq.size(); ++i) {
        complex<Real> dtft = Util::getDTFT(input, fq[i]);
        EXPECT_NEAR(1.0/nPoints, dtft.real(), 1e-2);
        EXPECT_NEAR(0.0, dtft.imag(), 1e-2);
    }
}

TEST(MathDTFTTest, gaussian) {
    const size_t nPoints = 10000;
    vector<Real> time = Util::linspace(pair<Real,Real>(-10,10), nPoints);
    vector<pair<Real,complex<Real>>> input(nPoints);
    Function::Gaussian gaussian((Real) 0.25, (Real) 0, (Real) 1.0);
    for (size_t i = 0; i < nPoints; ++i) {
        complex<Real> dataPoint(gaussian(time[i]), (Real) 0.0);
        input[i] = pair<Real,complex<Real>>(time[i], dataPoint);
    }

    const size_t nFq = 10;
    const Real samplingFq = nPoints / (time.back() - time.front());
    vector<Real> fq = Util::linspace(
            pair<Real,Real>(0, (nFq-1)*samplingFq/nPoints), nFq);
    for (size_t i = 0; i < fq.size(); ++i) {
        complex<Real> analytical = gaussian.getFourier(fq[i]);
        complex<Real> dtft = Util::getDTFT(input, fq[i]);
        EXPECT_NEAR(abs(analytical), abs(dtft), 1e-2);
        EXPECT_NEAR(arg(analytical), arg(dtft), 1e-2);
    }
}
