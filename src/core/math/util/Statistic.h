

#pragma once

#include <vector>

#include "math/Types.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
T meanDifference(const std::vector<T>& vec);

template<class T>
T mean(const std::vector<T>& vec);

template<class T>
T sum(const std::vector<T>& vec);

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

#include "Statistic.hpp"

