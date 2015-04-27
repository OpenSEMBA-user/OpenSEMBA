#include "RealUtils.h"

bool MathUtils::equal(const Real lhs, const Real rhs,
                      Real rel, const Real tol) {
    if (rel == 0.0) {
        rel = lhs+rhs;
    }
    if ((abs(lhs) < tol) && (abs(rhs) < tol)) {
        return true;
    } else if ((abs(lhs) < tol) || (abs(rhs) < tol)) {
        return false;
    } else if (abs(lhs-rhs) < tol*rel) {
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

bool MathUtils::greather(const Real lhs, const Real rhs,
                         Real rel, const Real tol) {
    return lower(rhs, lhs, rel, tol);
}

bool MathUtils::greatherEqual(const Real lhs, const Real rhs,
                              Real rel, const Real tol) {
    return !lower(lhs, rhs, rel, tol);
}
