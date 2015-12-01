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

#include "Tetrahedron10.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

const Math::Simplex::Triangle<2>    Tetrahedron10::tri;
const Math::Simplex::Tetrahedron<2> Tetrahedron10::tet;

Tetrahedron10::Tetrahedron10() {

}

Tetrahedron10::Tetrahedron10(const Id id,
                             const CoordR3* v[10],
                             const Layer* lay,
                             const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (Size i = 0; i < tet.np; i++) {
        v_[i] = v[i];
    }
    //
    check();
}

Tetrahedron10::Tetrahedron10(const Tetrahedron10& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (Size i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tetrahedron10::~Tetrahedron10() {

}

bool Tetrahedron10::isCurved() const {
    for (Size f = 0; f < tet.faces; f++) {
        if(isCurvedFace(f)) {
            return true;
        }
    }
    return false;
}

bool Tetrahedron10::isCurvedFace(const Size f) const {
    return getTri6Face(f).isCurved();
}

bool
Tetrahedron10::isFaceContainedInPlane(
        const Size face,
        const Math::Constants::CartesianPlane plane) const {
    return getTri6Face(face).isContainedInPlane(plane);
}

const CoordR3* Tetrahedron10::getSideV(const Size f, const Size i) const {
    return v_[tet.sideNode(f,i)];
}

const CoordR3*
Tetrahedron10::getVertex(const Size i) const {
    return v_[tet.vertex(i)];
}

const CoordR3* Tetrahedron10::getSideVertex(const Size f, const Size i) const {
    return v_[tet.sideVertex(f, i)];
}

Math::Real Tetrahedron10::getVolume() const {
    Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp];
    getCubatureJacobianDeterminant(cJDet);
    Math::Real res = 0.0;
    for (register Size c = 0; c < Math::Simplex::Tetrahedron<2>::ncp; c++) {
        res += tet.cw[c] * cJDet[c];
    }
    res *= Math::Real(1.0 / 6.0);
    return res;
}

Math::Real Tetrahedron10::getAreaOfFace(const Size f) const {
    return getTri6Face(f).getArea();
}

Triangle6 Tetrahedron10::getTri6Face(const Size f) const {
    const CoordR3* sideV[6];
    for (Size i = 0; i < 6; i++) {
        sideV[i] = getSideV(f,i);
    }
    Triangle6 auxFace(Id(0), sideV);
    return auxFace;
}

void Tetrahedron10::setV(const Size i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Tetrahedron4* Tetrahedron10::linearize() const {
    const CoordR3* vertex[4];
    for (Size i = 0; i < 4; i++) {
        vertex[i] = getVertex(i);
    }
    return new Tetrahedron4(getId(), vertex, getLayer(), getModel());
}

void Tetrahedron10::printInfo() const {
    std::cout << "--- Tet10 info ---" << std::endl;
    std::cout << "Id: " << getId() << std::endl;
    std::cout << "Coordinates:" << std::endl;
    for (Size i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

void Tetrahedron10::check() const {

}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
