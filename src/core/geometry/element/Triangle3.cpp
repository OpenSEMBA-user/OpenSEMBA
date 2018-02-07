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

#include "Triangle3.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Triangle<1> Triangle3::geo;

Triangle3::Triangle3() {

}

Triangle3::Triangle3(const Id id,
           const CoordR3* v[3],
           const Layer* lay,
           const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {
    for (std::size_t i = 0; i < geo.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Triangle3::Triangle3(const Triangle3& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {
    
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Triangle3::~Triangle3() {

}

const CoordR3* Triangle3::getSideV(const std::size_t f,
                                   const std::size_t i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[geo.sideNode(f, i)];
}

const CoordR3* Triangle3::getVertex(const std::size_t i) const {
    return v_[geo.vertex(i)];
}

const CoordR3* Triangle3::getSideVertex(const std::size_t f,
                                        const std::size_t i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideVertices());
    return v_[geo.sideVertex(f, i)];
}

Math::Real Triangle3::getArea() const {
    Math::CVecR3 v1, v2;
    v1 = getVertex(1)->pos() - getVertex(0)->pos();
    v2 = getVertex(2)->pos() - getVertex(0)->pos();
    return ((Math::Real) 0.5 * (v1 ^ v2).norm());
}

void Triangle3::setV(const std::size_t i, const CoordR3* vNew) {
    v_[i] = vNew;
}

void Triangle3::check() const {

}

void Triangle3::printInfo() const {
    std::cout << "--- Tri3 info ---" << std::endl;
    Base::printInfo();
    std::cout << "Coordinates:" << std::endl;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

Triangle3* Triangle3::linearize() const {
    return new Triangle3(*this);
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
