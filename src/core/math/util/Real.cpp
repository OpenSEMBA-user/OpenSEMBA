

#include "Real.h"

namespace SEMBA {
namespace Math {
namespace Util {

bool equal(const Real lhs, const Real rhs,
           Real rel, const Real tol) {
    if (rel == 0.0) {
        rel = std::abs(lhs+rhs);
    }
    if ((std::abs(lhs) <= epsilon) && (std::abs(rhs) <= epsilon)) {
        return true;
    } else if ((std::abs(lhs) <= epsilon) || (std::abs(rhs) <= epsilon)) {
        return false;
    } else if (std::abs(lhs-rhs) <= tol*rel) {
        return true;
    }
    return false;
}

bool notEqual(const Real lhs, const Real rhs,
              Real rel, const Real tol) {
    return !equal(lhs, rhs, rel, tol);
}

bool lower(const Real lhs, const Real rhs,
           Real rel, const Real tol) {
    if(equal(lhs, rhs, rel, tol)) {
        return false;
    }
    return lhs < rhs;
}

bool lowerEqual(const Real lhs, const Real rhs,
                Real rel, const Real tol) {
    return !lower(rhs, lhs, rel, tol);
}

bool greater(const Real lhs, const Real rhs,
             Real rel, const Real tol) {
    return lower(rhs, lhs, rel, tol);
}

bool greaterEqual(const Real lhs, const Real rhs,
                  Real rel, const Real tol) {
    return !lower(lhs, rhs, rel, tol);
}

Real ceil(const Real val, const Real rel) {
    Real low = floor(val);
    if (val <= (low + rel)) {
        return low;
    } else {
        return std::ceil(val);
    }
}

Real round(Real v) {
    Real dist = v - floor(v);
    if (greaterEqual(dist, 0.5)) {
        return ceil(v);
    } else {
        return floor(v);
    }
}

Real sign(Real val) {
    if (val < 0.0) {
        return -1.0;
    } else {
        return  1.0;
    }
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
