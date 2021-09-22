

#pragma once

#ifdef FFTW3_SUPPORT

#include <cmath>
#include <complex>
#include <utility>
#include <vector>
#include <fftw3.h>

#include "math/vector/Cartesian.h"
#include "math/Constants.h"


namespace SEMBA {
namespace Math {
namespace Util {

std::vector<std::pair<Real,std::complex<Real>>> getFFT(
        const std::vector<std::pair<Real,std::complex<Real>>>& data,
        const bool inverse = false);

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

#endif

