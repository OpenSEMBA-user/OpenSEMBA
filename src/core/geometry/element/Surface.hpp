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

#include "Surface.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

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
    for (std::size_t f = 0; f < 4; f++) {
        Math::Vector::Cartesian<T,3> p0 = 
            this->getSideVertex(f    %4,0)->pos();
        Math::Vector::Cartesian<T,3> p1 = 
            this->getSideVertex(f    %4,1)->pos();
        Math::Vector::Cartesian<T,3> p2 = 
            this->getSideVertex((f+1)%4,1)->pos();
        Math::Real sProd = (Math::Real)(p2-p1).dot(p1 - p0);
        if (Math::Util::greater(sProd, 0.0, 1.0)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Surface<T>::isContainedInPlane() const {
    return (isContainedInPlane(Math::Constants::xy) ||
            isContainedInPlane(Math::Constants::yz) ||
            isContainedInPlane(Math::Constants::zx));
}

template<class T>
bool Surface<T>::isContainedInPlane(
        const Math::Constants::CartesianPlane plane) const {
    // Checks if any vertex lies out of the plane.
    for (std::size_t i = 1; i < this->numberOfCoordinates(); i++) {
        if (!(*this->getV(i) - *this->getV(0)).isContainedInPlane(plane)) {
            return false;
        }
    }
    return true;
}

template<class T>
Math::Vector::Cartesian<T,3> Surface<T>::getNormal() const {
    Math::Vector::Cartesian<T,3> v0 = this->getVertex(1)->pos() -
                                      this->getVertex(0)->pos();
    Math::Vector::Cartesian<T,3> v1 = this->getVertex(2)->pos() -
                                      this->getVertex(0)->pos();
    return (v0 ^ v1).normalize();
}

template<class T>
void Surface<T>::printInfo() const {
    std::cout << " --- Surface Info ---" << std::endl;
    std::cout << getNormal() << std::endl;
    Element<T>::printInfo();
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
