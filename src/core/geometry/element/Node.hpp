

#include "Node.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
Node<T>::Node() {

}

template<class T>
Node<T>::Node(const Id id,
              const Coordinate::Coordinate<T,3>* v[1],
              const Layer* lay,
              const Model* mat)
:   Identifiable<Id>(id),
  Elem(lay, mat) {

    v_[0] = v[0];
}

template<class T>
Node<T>::Node(const Node<T>& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {

    v_[0] = rhs.v_[0];
}

template<class T>
Node<T>::~Node() {

}

template<class T>
bool Node<T>::isStructured(const Grid3& grid, const Math::Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isPoint()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
}

template<class T>
const Coordinate::Coordinate<T,3>* Node<T>::getV(const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Node<T>::getSideV(
        const std::size_t f,
        const std::size_t i) const {
    assert(f == 0 && i == 0);
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Node<T>::getVertex(
        const std::size_t i) const {
    assert(i == 0);
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Node<T>::getSideVertex(
        const std::size_t f,
        const std::size_t i) const {
    assert(f == 0 && i == 0);
    return v_[i];
}

template<class T>
void Node<T>::setV(const std::size_t i,
                   const Coordinate::Coordinate<T,3>* coord) {
    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
std::unique_ptr<ElemI> Node<T>::toStructured(
    const CoordI3Group& cG,
    const Grid3& grid, 
    const Math::Real tol) const {
    return std::make_unique<NodI>(this->getId(),
        this->vertexToStructured(cG, grid, tol).data(),
        this->getLayer(),
        this->getModel());
}

template<class T>
std::unique_ptr<ElemR> Node<T>::toUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    return std::make_unique<NodR>(this->getId(),
        this->vertexToUnstructured(cG, grid).data(),
        this->getLayer(),
        this->getModel());
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
