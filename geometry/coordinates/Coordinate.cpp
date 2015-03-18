/*
 * Coordinate.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */
#include "Coordinate.h"

Coordinate<void,0>::Coordinate() {

}

Coordinate<void,0>::Coordinate(const CoordinateId id)
:   ClassIdBase<CoordinateId>(id) {

}

Coordinate<void,0>::Coordinate(const Coordinate& rhs)
:   ClassIdBase<CoordinateId>(rhs) {

}

Coordinate<void,0>::~Coordinate() {

}

template<class T, Int D>
Coordinate<T,D>::Coordinate() {

}

template<class T, Int D>
Coordinate<T,D>::Coordinate(const CoordinateId id_,
                            const CartesianVector<T,D>& pos)
:   Coordinate<void,0>(id_),
    CartesianVector<T,D>(pos) {

}

template<class T, Int D>
Coordinate<T,D>::Coordinate(const CartesianVector<T,D>& pos)
:   CartesianVector<T,D>(pos) {

}

template<class T, Int D>
Coordinate<T,D>::~Coordinate() {

}

template <class T, Int D>
ClassBase* Coordinate<T,D>::clone() const {
   return new Coordinate<T,D>(*this);
}

template<class T, Int D>
Coordinate<T,D>& Coordinate<T,D>::operator=(const Coordinate& rhs) {
    if (this == &rhs)
        return *this;
    setId(rhs.getId());
    for (UInt i = 0; i < D; i++) {
        this->val[i] = rhs.val[i];
    }
    return *this;
}

template<class T, Int D>
bool Coordinate<T,D>::operator==(const Coordinate& rhs) const {
    return Coordinate<void,0>::operator==(rhs);
}

template<class T, Int D>
CartesianVector<T,3> Coordinate<T,D>::pos() const {
    CartesianVector<T,D> res;
    for (UInt i = 0; i < D; i++) {
        res.val[i] = this->val[i];
    }
    return res;
}

template<class T, Int D>
void
Coordinate<T,D>::printInfo() const {
    cout << "Id: ("<< getId() << ")  Pos: (";
    for (register Int i = 0; i < D; i++) {
        cout << this->val[i];
        if (i < D-1) {
            cout << " , ";
        }
    }
    cout << ")";
}

template class Coordinate<Real,3>;
template class Coordinate<Int ,3>;
