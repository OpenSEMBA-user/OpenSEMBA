

#include <math/util/DTFT.h>

namespace SEMBA {
namespace Math {
namespace Util {

std::complex<Real> getDTFT(
        const std::vector<std::pair<Real,std::complex<Real>>>& data,
        const Real f) {
    const std::size_t N = data.size();
    const std::complex<Real> imagUnit(0.0, 1.0);
    const Real normalizedFq = 2.0*Constants::pi * f;

    std::complex<Real> res(0.0, 0.0);
    for (std::size_t n = 0; n < N; n++) {
        res += data[n].second
                * std::exp(-imagUnit * normalizedFq * data[n].first);
    }
    res /= N;
    res *= data.back().first - data.front().first;
    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
