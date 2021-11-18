

#include "Hexahedron8.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
const Math::Real Hexahedron8<T>::tolerance = 1e-15;

template<class T>
Hexahedron8<T>::Hexahedron8() {

}

template<class T>
Hexahedron8<T>::Hexahedron8(const Id id,
                            const Coordinate::Coordinate<T,3>* v[8],
                            const Layer* lay,
                            const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = v[i];
    }
}

template<class T>
Hexahedron8<T>::Hexahedron8(
        Coordinate::Group<Coordinate::Coordinate<T,3> >& cG,
        const Id id,
        const Box<T,3>& box,
        const Layer* lay,
        const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    if(!box.isVolume()) {
        throw Geometry::Error::Box::NotVolume();
    }
    std::vector<Math::Vector::Cartesian<T,3> > pos = box.getPos();
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getPos(pos[i]);
        if (v_[i] == nullptr) {
            cG.addPos(pos[i]);
            v_[i] = cG.getPos(pos[i]);
        }
    }
}

template<class T>
Hexahedron8<T>::Hexahedron8(const Hexahedron8<T>& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
Hexahedron8<T>::~Hexahedron8() {

}

template<class T>
bool Hexahedron8<T>::isStructured(const Grid3& grid,
                                  const Math::Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isVolume()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
}

template<class T>
bool Hexahedron8<T>::isRegular() const {
    // Checks that all edges are aligned with one of the axis.
    static const Math::Vector::Cartesian<T,3> xAxe(1.0, 0.0, 0.0);
    static const Math::Vector::Cartesian<T,3> yAxe(0.0, 1.0, 0.0);
    static const Math::Vector::Cartesian<T,3> zAxe(1.0, 0.0, 1.0);
    for (std::size_t f = 0; f < numberOfFaces(); f++) {
        Math::Vector::Cartesian<T,3> first, second;
        Math::Vector::Cartesian<T,3> inc;
        for (std::size_t i = 0; i < numberOfSideVertices(); i++) {
            first = *getSideV(f, i);
            second = *getSideV(f, (i+1) % numberOfSideVertices());
            inc = (second - first).normalize();
            if (!((std::abs(inc.dot(xAxe))-1.0) <= tolerance ||
            (std::abs(inc.dot(yAxe))-1.0) <= tolerance ||
            (std::abs(inc.dot(zAxe))-1.0) <= tolerance)) {

                return false;
            }
        }
    }
    return true;
}

template<class T>
const Coordinate::Coordinate<T,3>* Hexahedron8<T>::getV(
        const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Hexahedron8<T>::getSideV(
const std::size_t f,
const std::size_t i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    static const std::array<
    std::array<std::size_t,4>,6>
    index = {{{0,1,2,3},
              {0,3,7,4},
              {0,4,5,1},
              {1,5,6,2},
              {2,6,7,3},
              {4,7,6,5}}};
    return v_[index[f][i]];
}

template<class T>
const Coordinate::Coordinate<T,3>* Hexahedron8<T>::getVertex(
const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Hexahedron8<T>::getSideVertex(
const std::size_t f,
const std::size_t i) const {
    return getSideV(f,i);
}

template<class T>
Math::Real Hexahedron8<T>::getAreaOfFace(const std::size_t f) const {
    Math::Vector::Cartesian<T,3> v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((Math::Real) 0.5 * (v1 ^ v2).norm());
}

template<class T>
Math::Real Hexahedron8<T>::getVolume() const {
    throw std::logic_error("Hexahedron8::getVolume() not implemented");
}

template<class T>
void Hexahedron8<T>::setV(const std::size_t i,
                          const Coordinate::Coordinate<T,3>* coord) {
    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
ElemI* Hexahedron8<T>::toStructured(const Coordinate::Group<CoordI3>& cG,
                                    const Grid3& grid,
                                    const Math::Real tol) const {
    const CoordI3** v = this->vertexToStructured(cG, grid, tol);
    if (v == nullptr) {
        return nullptr;
    }
    ElemI* res =  new HexI8(this->getId(),
                            v,
                            this->getLayer(),
                            this->getModel());
    delete[] v;
    return res;
}

template<class T>
ElemR* Hexahedron8<T>::toUnstructured(const Coordinate::Group<CoordR3>& cG,
                                      const Grid3& grid) const {
    const CoordR3** v = this->vertexToUnstructured(cG, grid);
    if (v == nullptr) {
        return nullptr;
    }
    ElemR* res =  new HexR8(this->getId(),
                            v,
                            this->getLayer(),
                            this->getModel());
    delete[] v;
    return res;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

