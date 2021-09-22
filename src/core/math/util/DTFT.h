

#pragma once

#include <cmath>
#include <complex>
#include <utility>
#include <vector>

#include "math/vector/Cartesian.h"
#include "math/Constants.h"

namespace SEMBA {
namespace Math {
namespace Util {

std::complex<Real> getDTFT(
        const std::vector<std::pair<Real,std::complex<Real>>>& data,
        const Real frequency);

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

