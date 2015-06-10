/*
 * Volume.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Volume.h"

template<class T>
Volume<T>::ErrorNullVolume::ErrorNullVolume(const ElementId& elemId) {
    stringstream aux;
    aux << "Element (" << elemId << ") has null volume";
    this->setMsg(aux.str());
}

template<class T>
Volume<T>::ErrorNullVolume::~ErrorNullVolume() throw () {

}

template<class T>
Volume<T>::ErrorSurfNotFound::ErrorSurfNotFound(const ElementId& volId,
                                                const ElementId& surfId) {
    stringstream aux;
    aux << "Surf " << surfId << " is not part of Volume " << volId;
    this->setMsg(aux.str());
}

template<class T>
Volume<T>::ErrorSurfNotFound::~ErrorSurfNotFound() throw () {

}

template<class T>
Volume<T>::Volume() {

}

template<class T>
Volume<T>::~Volume() {

}

template<class T>
bool Volume<T>::isLocalFace(const UInt f, const Surface<T>& surf) const {
	return sideNormal(f) == surf.getNormal();
}

template<class T>
bool Volume<T>::isFaceContainedInPlane(const UInt face,
                                       const CartesianPlane plane) const {
   Box<T,3> box = getBoundOfFace(face);
   CartesianVector<T,3> vec = box.getMax() - box.getMin();
   return vec.isContainedInPlane(plane);
}

template<class T>
CartesianVector<T,3> Volume<T>::sideNormal(const UInt f) const {
    CartesianVector<T,3> vec1, vec2, res;
	vec1 = *this->getSideVertex(f,1) - *this->getSideVertex(f,0);
	vec2 = *this->getSideVertex(f,2) - *this->getSideVertex(f,0);
	res = (vec1 ^ vec2).normalize();
	return res;
}

template<class T>
Box<T,3> Volume<T>::getBoundOfFace(const UInt face) const {
    Box<T,3> res;
   for (UInt i = 0; i < this->numberOfSideCoordinates(); i++) {
      res << this->getSideV(face,i)->pos();
   }
   return res;
}

template<class T>
UInt Volume<T>::getFaceNumber(const Surface<T>* surf) const {
    // Checks each face. Order is not important.
    for (UInt f = 0; f < this->numberOfFaces(); f++) {
        UInt vPresent = 0;
        for (UInt i = 0; i < surf->numberOfVertices(); i++) {
            for (UInt j = 0; j < surf->numberOfVertices(); j++) {
                if (surf->getVertex(j) == this->getSideVertex(f, i)) {
                    vPresent++;
                }
            }
            if (vPresent == surf->numberOfVertices()) {
                return f;
            }
        }
    }
    throw ErrorSurfNotFound(surf->getId(), this->getId());
}

template class Volume<Real>;
template class Volume<Int >;
