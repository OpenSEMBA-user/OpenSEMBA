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
    for (std::size_t f = 0; f < numberOfFaces(); f++) {
        for (std::size_t i = 0; i < face[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
    checkClosedness();
}

Polyhedron::Polyhedron(const Polyhedron& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    face_ = rhs.face_;
    for (std::size_t f = 0; f < numberOfFaces(); f++) {
        for (std::size_t i = 0; i < face_[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
}

Polyhedron::~Polyhedron() {

}

bool Polyhedron::isCurvedFace(const std::size_t f) const {
    return getFace(f)->isCurved();
}

std::size_t Polyhedron::numberOfCoordinates() const {
    return v_.size();
}

std::size_t Polyhedron::numberOfVertices() const {
    return numberOfCoordinates();
}

std::size_t Polyhedron::numberOfFaces() const {
    return face_.size();
}

std::size_t Polyhedron::numberOfSideVertices(const std::size_t f) const {
    assert(f < numberOfFaces());
    return face_[f]->numberOfVertices();
}

std::size_t Polyhedron::numberOfSideCoordinates(const std::size_t f) const {
    return numberOfSideVertices(f);
}

const CoordR3* Polyhedron::getV(
const std::size_t i) const {
    assert(i < numberOfCoordinates());
    return v_[i];
}

const CoordR3* Polyhedron::getSideV(
const std::size_t f,
const std::size_t i) const {
    return getFace(f)->getV(i);
}

const CoordR3* Polyhedron::getVertex(const std::size_t i) const {
    return getV(i);
}

const CoordR3* Polyhedron::getSideVertex(const std::size_t f,
                                         const std::size_t i) const {
    return getSideV(f,i);
}

const Polygon* Polyhedron::getFace(const std::size_t f) const {
    assert(f < numberOfFaces());
    return face_[f];
}

Math::Real Polyhedron::getAreaOfFace(const std::size_t f) const {
    return getFace(f)->getArea();
}

void Polyhedron::printInfo() const {
    std::cout << "--- Polyhedron info ---" << std::endl
              << "Number of vertices: " << numberOfVertices() << std::endl
              << "Number of faces: " << numberOfFaces() << std::endl;
    for (std::size_t i = 0; i < numberOfFaces(); i++) {
        std::cout<< "Face #" << i << std::endl;
        getFace(i)->printInfo();
    }
}

void Polyhedron::addV(const CoordR3* v) {
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        if (*v_[i] == *v) {
            return;
        }
    }
    v_.push_back(v);
}

void Polyhedron::checkClosedness() const {
    // This checks consists in pairing segments ordered according to their ids.
    // If a segment is not paired, the surface is not closed.
    std::size_t nSidesInFaces = 0;
    for (std::size_t f = 0; f < numberOfFaces(); f++) {
        nSidesInFaces += getFace(f)->numberOfFaces();
    }
    Math::Matrix::Dynamic<std::size_t> list(nSidesInFaces, 2);
    std::size_t row = 0;
    for (std::size_t f = 0; f < numberOfFaces(); f++) {
        for (std::size_t s = 0; s < getFace(f)->numberOfFaces(); s++) {
            std::size_t id0 = getFace(f)->getSideVertex(s,0)->getId().toInt();
            std::size_t id1 = getFace(f)->getSideVertex(s,1)->getId().toInt();
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
    for (std::size_t i = 0; i < nSidesInFaces; i += 2) {
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
