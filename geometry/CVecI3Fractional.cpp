/*
 * CoordIntFractional.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

using namespace std;

#include "CvecI3Fractional.h"

CvecI3Fractional::CvecI3Fractional() {
}

CvecI3Fractional& CvecI3Fractional::operator =(
        const CvecI3Fractional & rhs_) {
    CoordI3::operator =(rhs_);
    len_ = rhs_.len_;
    return *this;
}

bool CvecI3Fractional::less_coordI(const CvecI3Fractional &rhs) const {
    for (UInt n = 0; n < 3; n++) {
        if (this->val[n] < rhs.val[n]) {
            return true;
        } else if (this->val[n] > rhs.val[n]) {
            return false;
        }
    }
    return false;
}

bool CvecI3Fractional::less(const CvecI3Fractional &rhs,
        const Real tool) const {
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
        } else if (MathUtils::lower(len_.val[n], rhs.len_.val[n], tool)) {
            return false;
        }
    }
    return false;
}

bool CvecI3Fractional::cmp_coordI(const CvecI3Fractional &rhs) const {
    for (UInt n = 0; n < 3; n++) {
        if (this->val[n] != rhs.val[n]) {
            return false;
        }
    }
    return true;
}

bool CvecI3Fractional::cmp(const CvecI3Fractional &rhs,
        const Real tool) const {

    if (cmp_coordI(rhs)) {
        for (UInt n = 0; n < 3; n++) {
            if (MathUtils::equal(len_.val[n], rhs.len_.val[n], tool)) {
            } else {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

UInt CvecI3Fractional::getDirBase() {
    UInt rang = this->getRangeBase();
    if (rang == 1) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] > MathUtils::tolerance) {
                return n + 1;
            }
        }
    } else if (rang == 2) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] < MathUtils::tolerance) {
                return n + 1;
            }
        }
    }
    return 0;
}

//bool CoordIntFractional::IsInto(const Pixel& pixel) const {
//    return pixel.isInto(*this);
//}

UInt CvecI3Fractional::getRangeBase() {
    unsigned int rang;
    rang = 0;
    for (unsigned int n = 0; n < 3; ++n) {
        if (len_[n] < MathUtils::tolerance) {
            ++rang;
        }
    }
    return rang;
}

void CvecI3Fractional::applyTol(const double tol) {
    for (unsigned int n = 0; n < 3; ++n) {
        if (len_[n] < tol) {
            len_[n] = 0.0;
        } else if (len_[n] > (1.0 - tol)) {
            len_[n] = 0.0;
            ++val[n];
        }
    }
}

CVecI3 CvecI3Fractional::DiscretePositionDistribution(
        CVecI3 &numDivision) const {
    CVecI3 ret;
    for (UInt n = 0; n < 3; n++) {
        ret[n] = floor(len_[n] * ((Real) numDivision[n]));
    }
    return ret;
}

CVecR3 CvecI3Fractional::getRelativePos() const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos.val[n] = this->val[n] + len_.val[n];
    }
    return retPos;
}
