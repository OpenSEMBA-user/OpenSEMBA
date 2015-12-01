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
 * Surface.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */
#include "Surface.h"

template<class T>
Surface<T>::Surface() {

}

template<class T>
Surface<T>::~Surface() {

}

template<class T>
bool Surface<T>::isRectangular() const {
    if (this->numberOfCoordinates() != 4 || this->numberOfFaces() != 4) {
        return false;
    }
    for (UInt f = 0; f < 4; f++) {
        CartesianVector<T,3> p0 = this->getSideVertex(f % 4, 0)->pos();
        CartesianVector<T,3> p1 = this->getSideVertex(f % 4, 1)->pos();
        CartesianVector<T,3> p2 = this->getSideVertex((f+1) % 4, 1)->pos();
        Real sProd = (Real)(p2-p1).dot(p1 - p0);
        if (MathUtils::greater(sProd, 0.0, 1.0)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Surface<T>::isContainedInPlane() const {
    return (isContainedInPlane(xy) ||
            isContainedInPlane(yz) ||
            isContainedInPlane(zx));
}

template<class T>
bool Surface<T>::isContainedInPlane(const CartesianPlane plane) const {
    // Checks if any vertex lies out of the plane.
    for (UInt i = 1; i < this->numberOfCoordinates(); i++) {
        if (!(*this->getV(i) - *this->getV(0)).isContainedInPlane(plane)) {
            return false;
        }
    }
    return true;
}

template<class T>
CartesianVector<T,3> Surface<T>::getNormal() const {
    CartesianVector<T,3> v0 = this->getVertex(1)->pos() -
                              this->getVertex(0)->pos();
    CartesianVector<T,3> v1 = this->getVertex(2)->pos() -
                              this->getVertex(0)->pos();
    return (v0 ^ v1).normalize();
}

template<class T>
void Surface<T>::printInfo() const {
    cout << " --- Surface Info ---" << endl;
    cout << getNormal() << endl;
    Element<T>::printInfo();
}

template class Surface<Real>;
template class Surface<Int >;
