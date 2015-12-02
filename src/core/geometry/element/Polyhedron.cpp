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

#include "Polyhedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

Polyhedron::Polyhedron() {

}

Polyhedron::Polyhedron(const Id id,
                       const std::vector<const Polygon*>& face,
                       const Layer* lay,
                       const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    face_ = face;
    for (Size f = 0; f < numberOfFaces(); f++) {
        for (Size i = 0; i < face[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
    checkClosedness();
}

Polyhedron::Polyhedron(const Polyhedron& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    face_ = rhs.face_;
    for (Size f = 0; f < numberOfFaces(); f++) {
        for (Size i = 0; i < face_[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
}

Polyhedron::~Polyhedron() {

}

bool Polyhedron::isCurvedFace(const Size f) const {
    return getFace(f)->isCurved();
}

Size Polyhedron::numberOfCoordinates() const {
    return v_.size();
}

Size Polyhedron::numberOfVertices() const {
    return numberOfCoordinates();
}

Size Polyhedron::numberOfFaces() const {
    return face_.size();
}

Size Polyhedron::numberOfSideVertices(const Size f) const {
    assert(f < numberOfFaces());
    return face_[f]->numberOfVertices();
}

Size Polyhedron::numberOfSideCoordinates(const Size f) const {
    return numberOfSideVertices(f);
}

const CoordR3* Polyhedron::getV(
const Size i) const {
    assert(i < numberOfCoordinates());
    return v_[i];
}

const CoordR3* Polyhedron::getSideV(
const Size f,
const Size i) const {
    return getFace(f)->getV(i);
}

const CoordR3* Polyhedron::getVertex(const Size i) const {
    return getV(i);
}

const CoordR3* Polyhedron::getSideVertex(const Size f, const Size i) const {
    return getSideV(f,i);
}

const Polygon* Polyhedron::getFace(const Size f) const {
    assert(f < numberOfFaces());
    return face_[f];
}

Math::Real Polyhedron::getAreaOfFace(const Size f) const {
    return getFace(f)->getArea();
}

void Polyhedron::printInfo() const {
    std::cout << "--- Polyhedron info ---" << std::endl
              << "Number of vertices: " << numberOfVertices() << std::endl
              << "Number of faces: " << numberOfFaces() << std::endl;
    for (Size i = 0; i < numberOfFaces(); i++) {
        std::cout<< "Face #" << i << std::endl;
        getFace(i)->printInfo();
    }
}

void Polyhedron::addV(const CoordR3* v) {
    for (Size i = 0; i < numberOfCoordinates(); i++) {
        if (*v_[i] == *v) {
            return;
        }
    }
    v_.push_back(v);
}

void Polyhedron::checkClosedness() const {
    // This checks consists in pairing segments ordered according to their ids.
    // If a segment is not paired, the surface is not closed.
    Size nSidesInFaces = 0;
    for (Size f = 0; f < numberOfFaces(); f++) {
        nSidesInFaces += getFace(f)->numberOfFaces();
    }
    Math::Matrix::Dynamic<Size> list(nSidesInFaces, 2);
    Size row = 0;
    for (Size f = 0; f < numberOfFaces(); f++) {
        for (Size s = 0; s < getFace(f)->numberOfFaces(); s++) {
            Size id0 = getFace(f)->getSideVertex(s,0)->getId().toInt();
            Size id1 = getFace(f)->getSideVertex(s,1)->getId().toInt();
            if (id0 < id1) {
                list(row,0) = id0;
                list(row,1) = id1;
            } else {
                list(row,0) = id1;
                list(row,1) = id0;
            }
            row++;
        }
    }
    list.sortRows(0,1);
    for (Size i = 0; i < nSidesInFaces; i += 2) {
        if (list(i,0) != list(i+1,0) || list(i,1) != list(i+1,1)) {
            throw Error::NotClosed();
        }
    }
}

Math::Real Polyhedron::getVolume() const {
    throw std::logic_error("Polyhedron::getVolume() not implemented");
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
