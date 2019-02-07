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

#include "Quadrilateral4.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
Quadrilateral4<T>::Quadrilateral4() {

}

template<class T>
Quadrilateral4<T>::Quadrilateral4(const Id id,
                                  const Coordinate::Coordinate<T,3>* coords[4],
                                  const Layer* lay,
                                  const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {
    
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = coords[i];
    }
    // TODO Normals are not handled.
    check();
}

template<class T>
Quadrilateral4<T>::Quadrilateral4(
        Coordinate::Group<Coordinate::Coordinate<T,3> >& cG,
        const Id id,
        const Box<T,3>& box,
        const Layer* lay,
        const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    if(!box.isSurface()) {
        throw Geometry::Error::Box::NotSurface();
    }
    std::vector<Math::Vector::Cartesian<T,3> > pos = box.getPos();
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getPos(pos[i]);
        if (v_[i] == nullptr) {
            v_[i] = cG.addPos(pos[i]);
        }
    }
}

template<class T>
Quadrilateral4<T>::Quadrilateral4(const Quadrilateral4<T>& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {
    
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
Quadrilateral4<T>::~Quadrilateral4() {
    
}

template<class T>
bool Quadrilateral4<T>::isStructured(const Grid3& grid,
                                     const Math::Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isSurface()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
}

template<class T>
const Coordinate::Coordinate<T,3>* Quadrilateral4<T>::getV(
        const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Quadrilateral4<T>::getVertex(
        const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Quadrilateral4<T>::getSideV(
        const std::size_t f,
        const std::size_t i) const {
    assert(f < this->numberOfFaces());
    assert(i < numberOfSideCoordinates());
    return v_[(f + i) % 4];
}

template<class T>
const Coordinate::Coordinate<T,3>* Quadrilateral4<T>::getSideVertex(
        const std::size_t f,
        const std::size_t i) const {
    assert(f < this->numberOfFaces());
    assert(i < this->numberOfSideVertices());
    return v_[(f + i) % 4];
}

template<class T>
void Quadrilateral4<T>::setV(const std::size_t i,
                             const Coordinate::Coordinate<T,3>* coord) {
    v_[i] = coord;
}

template<class T>
ElemI* Quadrilateral4<T>::toStructured(
        const Coordinate::Group<CoordI3>& cG,
        const Grid3& grid, const Math::Real tol) const {
    const CoordI3** coords = this->vertexToStructured(cG, grid, tol);
    if (coords == nullptr) {
        return nullptr;
    }
    ElemI* res =  new QuaI4(this->getId(),
                            coords,
                            this->getLayer(),
                            this->getModel());
    delete[] coords;
    return res;
}

template<class T>
ElemR* Quadrilateral4<T>::toUnstructured(
        const Coordinate::Group<CoordR3>& cG,
        const Grid3& grid) const {
    const CoordR3** coords = this->vertexToUnstructured(cG, grid);
    if (coords == nullptr) {
        return nullptr;
    }
    ElemR* res =  new QuaR4(this->getId(),
                            coords,
                            this->getLayer(),
                            this->getModel());
    delete[] coords;
    return res;
}

template<class T>
void Quadrilateral4<T>::printInfo() const {
    std::cout << "--- Quad4 info ---" << std::endl;
    Quadrilateral<T>::printInfo();
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

template<class T>
void Quadrilateral4<T>::check() const {

}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
