#include "RealUtils.h"

bool MathUtils::equal(const Real lhs, const Real rhs,
                      Real rel, const Real tol) {
    if (rel == 0.0) {
        rel = abs(lhs+rhs);
    }
    if ((abs(lhs) <= epsilon) && (abs(rhs) <= epsilon)) {
        return true;
    } else if ((abs(lhs) <= epsilon) || (abs(rhs) <= epsilon)) {
        return false;
    } else if (abs(lhs-rhs) <= tol*rel) {
        return true;
    }
    return false;
}

bool MathUtils::notEqual(const Real lhs, const Real rhs,
                         Real rel, const Real tol) {
    return !equal(lhs, rhs, rel, tol);
}

bool MathUtils::lower(const Real lhs, const Real rhs,
                      Real rel, const Real tol) {
    if(equal(lhs, rhs, rel, tol)) {
        return false;
    }
    return lhs < rhs;
}

bool MathUtils::lowerEqual(const Real lhs, const Real rhs,
                           Real rel, const Real tol) {
    return !lower(rhs, lhs, rel, tol);
}

bool MathUtils::greater(const Real lhs, const Real rhs,
                         Real rel, const Real tol) {
    return lower(rhs, lhs, rel, tol);
}

bool MathUtils::greaterEqual(const Real lhs, const Real rhs,
                             Real rel, const Real tol) {
    return !lower(lhs, rhs, rel, tol);
}

Real MathUtils::ceil(const Real val, const Real rel) {
    Real low = floor(val);
    if (val <= (low + rel)) {
        return low;
    } else {
        return std::ceil(val);
    }
}

Real MathUtils::round(Real v) {
    Real dist = v - floor(v);
    if (greaterEqual(dist, 0.5)) {
        return ceil(v);
    } else {
        return floor(v);
    }
}
