/*
 * Coordinate.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */
#include "Coordinate.h"

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

template <class T, Int D>
ClassBase* Coordinate<T,D>::clone() const {
   return new Coordinate<T,D>(*this);
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
bool Coordinate<T,D>::operator==(const Coordinate& rhs) const {
    return ClassIdBase<CoordinateId>::operator==(rhs);
}

template<class T, Int D>
CartesianVector<T,D> Coordinate<T,D>::pos() const {
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
    cout << ")";
}

template class Coordinate<Real,3>;
template class Coordinate<Int ,3>;
