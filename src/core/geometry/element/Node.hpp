

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
Node<T>::Node(Coordinate::Group<Coordinate::Coordinate<T,3> >& cG,
              const Id id,
              const Box<T,3>& box,
              const Layer* lay,
              const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    if(!box.isPoint()) {
        throw Geometry::Error::Box::NotPoint();
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
ElemI* Node<T>::toStructured(const Coordinate::Group<CoordI3>& cG,
                             const Grid3& grid, const Math::Real tol) const {
    const CoordI3** v = this->vertexToStructured(cG, grid, tol);
    if (v == nullptr) {
        return nullptr;
    }
    ElemI* res =  new NodI(this->getId(),
                           v,
                           this->getLayer(),
                           this->getModel());
    delete[] v;
    return res;
}

template<class T>
ElemR* Node<T>::toUnstructured(const Coordinate::Group<CoordR3>& cG,
                               const Grid3& grid) const {
    const CoordR3** v = this->vertexToUnstructured(cG, grid);
    if (v == nullptr) {
        return nullptr;
    }
    ElemR* res =  new NodR(this->getId(),
                           v,
                           this->getLayer(),
                           this->getModel());
    delete[] v;
    return res;
}

template<class T>
void Node<T>::printInfo() const {
    std::cout << "--- Node info ---" << std::endl;
    Element<T>::printInfo();
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
