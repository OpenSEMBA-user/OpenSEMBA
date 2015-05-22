/*
 * CoordIntFractional.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

using namespace std;

#include "CVecI3Fractional.h"

CVecI3Fractional::CVecI3Fractional() {
}

CVecI3Fractional& CVecI3Fractional::operator =(
        const CVecI3Fractional & rhs) {
    CVecI3::operator =(rhs);
    len_ = rhs.len_;
    return *this;
}

bool CVecI3Fractional::less(const CVecI3Fractional &rhs,
        const Real tol) const {
    for (UInt n = 0; n < 3; n++) {
        if (this->val[n] < rhs.val[n]) {
            return true;
        } else if (this->val[n] > rhs.val[n]) {
            return false;
        }
    }
    for (UInt n = 0; n < 3; n++) {
        if (this->len_.val[n] < rhs.len_.val[n]) {
            return true;
        } else if (MathUtils::lower(len_.val[n], rhs.len_.val[n], tol)) {
            return false;
        }
    }
    return false;
}

CVecI3Frac::Direction CVecI3Fractional::getDirBase() const {
    UInt rang = this->getRangeBase();
    if (rang == 1) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] > MathUtils::tolerance) {
                return Direction(n + 1);
            }
        }
    } else if (rang == 2) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] < MathUtils::tolerance) {
                return Direction(n + 1);
            }
        }
    }
    return dirNode;
}

UInt CVecI3Fractional::getRangeBase() const {
    unsigned int rang;
    rang = 0;
    for (unsigned int n = 0; n < 3; ++n) {
        if (len_[n] < MathUtils::tolerance) {
            ++rang;
        }
    }
    return rang;
}

void CVecI3Fractional::applyTol(const double tol) {
    for (unsigned int n = 0; n < 3; ++n) {
        if (len_[n] < tol) {
            len_[n] = 0.0;
        } else if (len_[n] > (1.0 - tol)) {
            len_[n] = 0.0;
            ++val[n];
        }
    }
}

CVecI3 CVecI3Fractional::DiscretePositionDistribution(
        CVecI3 &numDivision) const {
    CVecI3 ret;
    for (UInt n = 0; n < 3; n++) {
        ret[n] = floor(len_[n] * ((Real) numDivision[n]));
    }
    return ret;
}

CVecR3 CVecI3Fractional::getRelativePos() const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos.val[n] = this->val[n] + len_.val[n];
    }
    return retPos;
}
