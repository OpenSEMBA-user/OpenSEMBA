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

bool CoordinateConformal::operator==(const CoordinateBase& rhs) const {
    if (!Coordinate<Int,3>::operator==(rhs)) {
        return false;
    }
    const CoordinateConformal* rhsPtr = rhs.castTo<CoordinateConformal>();
    bool res = true;
    res &= (this->length_ == rhsPtr->length_);
    res &= (this->dir_ == rhsPtr->dir_);
    return res;
}

CoordR3* CoordinateConformal::toUnstructured(const Grid3& grid) const {
    CVecR3 pos = grid.getPos(*this);
    if (MathUtils::greater(getLength(), 0.0)) {
        Int dir = getDir();
        Real length = getLength();
        CVecI3 cellAux = *this;
        cellAux(dir)++;
        CVecR3 posAux = grid.getPos(cellAux);
        Real step = posAux(dir)-pos(dir);
        pos(dir) += step*length;
    }
    return new CoordR3(this->getId(), pos);
}

void CoordinateConformal::printInfo() const {
    CoordI3::printInfo();
    cout << " Dir: (";
    if(MathUtils::equal(length_, 0.0)) {
        cout << "0";
    } else {
        cout << ('x'+dir_);
    }
    cout << ")";
    if(MathUtils::notEqual(length_, 0.0)) {
        cout << " Length: (" << length_ << ")";
    }
}
