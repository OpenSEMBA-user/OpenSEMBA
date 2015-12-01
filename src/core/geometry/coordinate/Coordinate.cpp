// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
Coordinate<T,D>* Coordinate<T,D>::clone() const {
    return new Coordinate<T,D>(*this);
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
bool Coordinate<T,D>::isStructured(const Grid<D>& grid, const Real tol) const {
    if (!grid.isCell(*this, tol)) {
        return false;
    }
    return true;
}

template<class T, Int D>
Coordinate<Int,D>* Coordinate<T,D>::toStructured(const Grid<D>& grid) const {
    return new Coordinate<Int,D>(this->getId(), grid.getCell(*this));
}

template<class T, Int D>
Coordinate<Real,D>* Coordinate<T,D>::toUnstructured(const Grid<D>& grid) const {
    return new Coordinate<Real,D>(this->getId(), grid.getPos(*this));
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
template class Coordinate<Int ,3>;
