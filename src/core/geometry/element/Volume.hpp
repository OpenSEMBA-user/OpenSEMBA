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

#include "Volume.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
Volume<T>::Volume() {

}

template<class T>
Volume<T>::~Volume() {

}

template<class T>
bool Volume<T>::isLocalFace(const std::size_t f,
                            const Surface<T>& surf) const {
    return getSideNormal(f) == surf.getNormal();
}

template<class T>
bool Volume<T>::isFaceContainedInPlane(
        const std::size_t face,
        const Math::Constants::CartesianPlane plane) const {
   Box<T,3> box = getBoundOfFace(face);
   Math::Vector::Cartesian<T,3> vec = box.getMax() - box.getMin();
   return vec.isContainedInPlane(plane);
}

template<class T>
Math::Vector::Cartesian<T,3> Volume<T>::getSideNormal(
        const std::size_t f) const {
    Math::Vector::Cartesian<T,3> vec1, vec2, res;
    vec1 = *this->getSideVertex(f,1) - *this->getSideVertex(f,0);
    vec2 = *this->getSideVertex(f,2) - *this->getSideVertex(f,0);
    res = (vec1 ^ vec2).normalize();
    return res;
}

template<class T>
Box<T,3> Volume<T>::getBoundOfFace(const std::size_t face) const {
    Box<T,3> res;
   for (std::size_t i = 0; i < this->numberOfSideCoordinates(); i++) {
      res << this->getSideV(face,i)->pos();
   }
   return res;
}

template<class T>
std::size_t Volume<T>::getFaceNumber(const Surface<T>* surf) const {
    // Checks each face. Order is not important.
    for (std::size_t f = 0; f < this->numberOfFaces(); f++) {
        std::size_t vPresent = 0;
        for (std::size_t i = 0; i < surf->numberOfVertices(); i++) {
            for (std::size_t j = 0; j < surf->numberOfVertices(); j++) {
                if (surf->getVertex(j) == this->getSideVertex(f, i)) {
                    vPresent++;
                }
            }
            if (vPresent == surf->numberOfVertices()) {
                return f;
            }
        }
    }
    throw Error::SurfNotFound(surf->getId(), this->getId());
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
