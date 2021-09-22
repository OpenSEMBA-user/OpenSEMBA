

#include "SpaceGenerator.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
std::vector<Real> logspace(const std::pair<Real,Real>& rangeExponents,
                           const T nPoints) {
    std::vector<Real> res;
    const Real base = (Real) 10;
    res.reserve(nPoints);
    if (nPoints == 1) {
        res.push_back(pow(base, rangeExponents.second));
    } else {
        Real jump =
        (rangeExponents.second - rangeExponents.first)
        / (Real) (nPoints - 1);
        for (std::size_t i = 0; i < nPoints; i++) {
            res.push_back(
            pow(base, rangeExponents.first + (Real) i * jump));
        }
    }
    return res;
}

template<class T>
std::vector<T> linspace(const std::pair<T,T>& range,
                        const std::size_t nPoints) {
    std::vector<T> res;
    res.reserve(nPoints);
    if (nPoints == 1) {
        res.push_back(range.second);
    } else {
        T jump = (range.second - range.first) / (T) (nPoints - 1);
        for (std::size_t i = 0; i < nPoints; i++) {
            res.push_back(range.first + (T) i * jump);
        }
    }
    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
