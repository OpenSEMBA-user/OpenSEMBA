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
        if (len_[n] > MathUtils::tolerance) {
            ++rang;
        }
    }
    return rang;
}

CVecI3 CVecI3Fractional::DiscretePositionDistribution(
        const CVecI3 &numDivision) const {
    CVecI3 ret;
    for (UInt n = 0; n < 3; n++) {
        ret[n] = floor(len_[n] * ((Real) numDivision[n]));
    }
    return ret;
}

CVecI3 CVecI3Fractional::DiscretePositionDistribution(
        const CVecI3 &numDivision, const CVecI3 &origin) const {
    CVecI3 ret(-1);
    for (UInt n = 0; n < 3; n++) {
        ret[n] = floor((len_[n]+(Real)(val[n]-origin [n]) )*
                ((Real) numDivision[n]));
        if(ret[n]<0){
            ret[n] = 0;
        }else if (ret[n]>=numDivision[n]){
            ret[n] = numDivision[n]-1;
        }
    }
    return ret;
}

CVecR3 CVecI3Fractional::getScalePos() const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos.val[n] = (Real)val[n] + len_.val[n];
    }
    return retPos;
}

CVecR3 CVecI3Fractional::getScalePos(const CVecI3 origin) const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos.val[n] = (Real)(val[n]-origin[n]) + len_.val[n];
    }
    return retPos;
}


string CVecI3Fractional::toStr() const {
    return CVecI3::toStr() + " len: " + len_.toStr();
}

bool CVecI3Fractional::operator ==(const CVecI3Fractional& rhs) const {
    return (CVecI3::operator ==(rhs) && len_ == rhs.len_);
}
