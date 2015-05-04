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
