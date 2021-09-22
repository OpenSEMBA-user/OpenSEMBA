

#pragma once

#include <cmath>
#include <limits>

#include "math/Types.h"

namespace SEMBA {
namespace Math {
namespace Util {

const Real epsilon = std::numeric_limits<Real>::epsilon()*1.0e2;
const Real tolerance = 1e-10;

bool equal       (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool notEqual    (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool lower       (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool lowerEqual  (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool greater     (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool greaterEqual(const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);

Real sign(Real val);
Real ceil(const Real v, Real rel = 0.0);
Real round(Real v);

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

