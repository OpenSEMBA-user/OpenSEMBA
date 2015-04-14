/*
 * Coordinate.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */
#include "Coordinate.h"


CoordinateBase::CoordinateBase() {

}

CoordinateBase::~CoordinateBase() {

}

bool CoordinateBase::operator ==(const CoordinateBase& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

bool CoordinateBase::operator !=(const CoordinateBase& rhs) const {
    return !(*this == rhs);
}

template<class T, Int D>
Coordinate<T,D>::Coordinate() {

}

template<class T, Int D>
Coordinate<T,D>::Coordinate(const CoordinateId id,
                            const CartesianVector<T,D>& pos)
:   ClassIdBase<CoordinateId>(id),
    CartesianVector<T,D>(pos) {

}

template<class T, Int D>
Coordinate<T,D>::Coordinate(const CartesianVector<T,D>& pos)
:   CartesianVector<T,D>(pos) {

}

template<class T, Int D>
Coordinate<T,D>::Coordinate(const Coordinate& rhs)
:   ClassIdBase<CoordinateId>(rhs),
    CartesianVector<T,D>(rhs) {

}

template<class T, Int D>
Coordinate<T,D>::~Coordinate() {

}

template<class T, Int D>
Coordinate<T,D>& Coordinate<T,D>::operator=(const Coordinate& rhs) {
    if (this == &rhs)
        return *this;
    setId(rhs.getId());
    CartesianVector<T,D>::operator=(rhs);

    return *this;
}

template<class T, Int D>
bool Coordinate<T,D>::operator==(const CoordinateBase& rhs) const {
    if(!CoordinateBase::operator==(rhs)) {
        return false;
    }
    const Coordinate<T,D>* rhsPtr = rhs.castTo<Coordinate<T,D> >();
    bool res = true;
    res &= (this->getId() == rhsPtr->getId());
    res &= (this->pos() == rhsPtr->pos());
    return res;
}

template<class T, Int D>
bool Coordinate<T,D>::operator!=(const CoordinateBase& rhs) const {
    return CoordinateBase::operator!=(rhs);
}

template<class T, Int D>
CartesianVector<T,D>& Coordinate<T,D>::pos() {
    return *this;
}

template<class T, Int D>
const CartesianVector<T,D>& Coordinate<T,D>::pos() const {
    return *this;
}

template<class T, Int D>
void Coordinate<T,D>::printInfo() const {
    cout << "Id: ("<< getId() << ")  Pos: (";
    for (register Int i = 0; i < D; i++) {
        cout << this->val[i];
        if (i < D-1) {
            cout << " , ";
        }
    }
    cout << ")" << endl;
}

template class Coordinate<Real,3>;
template class Coordinate<Int, 3> ;
