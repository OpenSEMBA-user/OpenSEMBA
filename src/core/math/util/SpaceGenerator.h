

#pragma once

#include <utility>
#include <vector>

#include "math/Types.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
std::vector<Real> logspace(const std::pair<Real, Real>& rangeExponents,
                           const T nPoints);

template<class T>
std::vector<T> linspace(const std::pair<T,T>& range,
                        const std::size_t nPoints);

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

#include "SpaceGenerator.hpp"

