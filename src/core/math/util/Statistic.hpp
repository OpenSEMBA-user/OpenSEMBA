

#include "Statistic.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
T meanDifference(const std::vector<T>& vec) {
    if (vec.size() == 1) {
        return (T) 0.0;
    }
    const std::size_t vS = vec.size();
    std::vector<T> diff(vS-1, 0.0);
    for (std::size_t i = 1; i < vS; i++) {
        diff[i-1] = vec[i] - vec[i-1];
    }
    return mean(diff);
}

template<class T>
T mean(const std::vector<T>& vec) {
    const std::size_t vS = vec.size();
    return sum(vec) / vS;
}

template<class T>
T sum(const std::vector<T>& vec) {
    const std::size_t vS = vec.size();
    T res = 0.0;
    for (std::size_t i = 0; i < vS; i++) {
        res += vec[i];
    }
    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
