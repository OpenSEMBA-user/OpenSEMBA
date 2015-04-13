/*
 * CoordinateConformal.cpp
 *
 *  Created on: 21/3/2015
 *      Author: Daniel
 */
#include "CoordinateConformal.h"

CoordinateConformal::CoordinateConformal() {
    dir_ = x;
    length_ = 0.0;
}

CoordinateConformal::CoordinateConformal(const CoordinateId id,
                                         const CVecI3& pos,
                                         const CartesianAxis dir,
                                         const Real length)
:   ClassIdBase<CoordinateId>(id),
    CVecI3(pos) {

    dir_    = dir;
    length_ = length;
}

CoordinateConformal::CoordinateConformal(const CartesianAxis dir,
                                         const Real length) {

    dir_    = dir;
    length_ = length;
}

CoordinateConformal::CoordinateConformal(const CoordinateConformal& rhs)
:   ClassIdBase<CoordinateId>(rhs),
    CVecI3(rhs) {

    dir_    = rhs.dir_;
    length_ = rhs.length_;
}

CoordinateConformal::~CoordinateConformal() {

}

CoordinateConformal& CoordinateConformal::operator=(
        const CoordinateConformal& rhs) {
    if (this == &rhs)
        return *this;

    CoordI3::operator=(rhs);
    dir_    = rhs.dir_;
    length_ = rhs.length_;

    return *this;
}

bool CoordinateConformal::operator<(const ClassCompBase& rhs) const {
    if (!Coordinate<Int,3>::operator<(rhs)) {
        return false;
    }
    const CoordinateConformal* rhsPtr = rhs.castTo<CoordinateConformal>();
    if (!(this->length_ < rhsPtr->length_)) {
        return false;
    }
    return (this->dir_ < rhsPtr->dir_);
}

void CoordinateConformal::printInfo() const {
    CoordI3::printInfo();
    cout << " Dir: (";
    if(MathUtils::equal(length_, 0.0, 1.0)) {
        cout << "0";
    } else {
        cout << ('x'+dir_);
    }
    cout << ")";
    if(MathUtils::notEqual(length_, 0.0, 1.0)) {
        cout << " Length: (" << length_ << ")";
    }
}
