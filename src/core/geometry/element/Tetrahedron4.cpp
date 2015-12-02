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

#include "Tetrahedron4.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Tetrahedron<1> Tetrahedron4::tet;

Tetrahedron4::Tetrahedron4() {

}

Tetrahedron4::Tetrahedron4(const Id id,
           const CoordR3* v[4],
           const Layer* lay,
           const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (Size i = 0; i < tet.np; i++) {
        v_[i] = v[i];
    }
    check();
}

Tetrahedron4::Tetrahedron4(const Tetrahedron4& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (Size i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tetrahedron4::~Tetrahedron4() {

}

bool Tetrahedron4::isInnerPoint(const Math::CVecR3& pos) const {
    if (!getBound().isInnerPoint(pos)) {
        return false;
    }
    // Checks if point is inside a tetrahedron using the following algorithm:
    // http://steve.hollasch.net/cgindex/geometry/ptintet.html
    Math::Matrix::Static<Math::Real,4,4> mat;
    // Builds matrix D0.
    for (Size i = 0; i < 4; i++) {
        for (Size j = 0; j < 3; j++) {
            mat(i,j) = getVertex(i)->pos()(j);
        }
        mat(i,3) = (Math::Real) 1.0;
    }
    Math::Real det = mat.getDeterminant4x4();
    assert(det != 0);
    bool isPositive = (det > (Math::Real) 0.0);
    // Checks rest of matrices. Last column is always 1.0.
    for (Size k = 0; k < 4; k++) {
        // Copies pos in row k.
        for (Size j = 0; j < 3; j++) {
            mat(k,j) = pos(j);
        }
        // Copies rest of vertices.
        for (Size i = 0; i < 4; i++) {
            if (i != k) {
                for (Size j = 0; j < 3; j++) {
                    mat(i,j) = getVertex(i)->pos()(j);
                }
            }
        }
        Math::Real det = mat.getDeterminant4x4();
        if ((det > (Math::Real) 0.0) != isPositive) {
            return false;
        }
    }
    return true;
}

bool Tetrahedron4::isCurvedFace(const Size face) const {
    return false;
}

bool Tetrahedron4::isFaceContainedInPlane(
        const Size face,
        const Math::Constants::CartesianPlane plane) const {
    return getTri3Face(face)->isContainedInPlane(plane);
}

const CoordR3* Tetrahedron4::getSideV(const Size f, const Size i) const {
    return v_[tet.sideNode(f,i)];
}

const CoordR3* Tetrahedron4::getSideVertex(const Size f, const Size i) const {
    return v_[tet.sideVertex(f,i)];
}

Math::Real Tetrahedron4::getVolume() const {
    Math::Matrix::Static<Math::Real,3,3> mat;
    Math::CVecR3 aux;
    for (Size i = 1; i < 4; i++) {
        aux = getV(0)->pos() - getV(i)->pos();
        for (Size j = 0; j < 3; j++) {
            mat(i-1,j) = aux(j);
        }
    }
    Math::Real det = mat.getDeterminant3x3();
    return (det / ((Math::Real) 6.0));
}

Math::Real Tetrahedron4::getAreaOfFace(const Size f) const {
    Math::CVecR3 v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((Math::Real) 0.5 * (v1 ^ v2).norm());
}

void Tetrahedron4::setV(const Size i, const CoordR3* v) {
    v_[i] = v;
}

void Tetrahedron4::check() const {
    if(hasZeroVolume()) {
        throw Error::NullVolume(this->getId());
    }
}

void Tetrahedron4::printInfo() const {
    std::cout << "--- Tet4 info ---" << std::endl;
    std::cout << "Id: " << getId() << std::endl;
    std::cout << "Coordinates:" << std::endl;
    for (Size i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

bool Tetrahedron4::hasZeroVolume() const {
    bool zeroVolume;
    Math::CVecR3 initialVCoord, otherVCoord;
    initialVCoord = *v_[0];
    for (Size d = 0; d < 3; d++) {
        zeroVolume = true;
        for (Size i = 1; i < tet.np; i++) {
            otherVCoord = *v_[i];
            zeroVolume &= (initialVCoord(d) == otherVCoord(d));
        }
        if (zeroVolume) {
            return true;
        }
    }
    return false;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
