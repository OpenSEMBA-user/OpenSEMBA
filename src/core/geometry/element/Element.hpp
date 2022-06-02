

#include "Element.h"

#include <algorithm>

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
Element<T>::Element() {

}

template<class T>
Element<T>::~Element() {

}

template<class T>
bool Element<T>::operator==(const Base& rhs) const {
    if (!Base::operator==(rhs)) {
        return false;
    }
    const Element<T>* rhsPtr = rhs.castTo<Element<T>>();
    bool res = true;
    res &= (this->getId() == rhsPtr->getId());
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        res &= (*this->getV(i) == *rhsPtr->getV(i));
    }
    res &= (this->getLayerId() == rhsPtr->getLayerId());
    res &= (this->getMatId() == rhsPtr->getMatId());

    return res;
}

template<class T>
bool Element<T>::isCoordinate(const Coordinate::Coordinate<T,3>* coord) const {
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        if (*getV(i) == *coord) {
            return true;
        }
    }
    return false;
}

template<class T>
bool Element<T>::isStructured(const Grid3& grid, const Math::Real tol) const {
    return false;
}

template<class T>
bool Element<T>::isInnerPoint(const Math::Vector::Cartesian<T,3>& pos) const {
    throw std::logic_error("Element::isInnerPoint not implemented");
    return false;
}

template<class T>
Box<T,3> Element<T>::getBound() const {
    Box<T,3> res;
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        res << getV(i)->pos();
    }
    return res;
}

template<class T>
const Coordinate::Coordinate<T,3>* Element<T>::getMinV() const {
    assert(getV(0) != nullptr);
    const Coordinate::Coordinate<T,3>* res = getVertex(0);
    for (std::size_t i = 1; i < numberOfVertices(); i++) {
        if(res->pos() == getVertex(i)->pos()) {
            continue;
        }
        for (std::size_t j = 0; j < 3; j++) {
            Math::Real val1 = getVertex(i)->pos()(j);
            Math::Real val2 = res->pos()(j);
            if(Math::Util::lower(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
const Coordinate::Coordinate<T,3>* Element<T>::getMaxV() const {
    assert(getV(0) != nullptr);
    const Coordinate::Coordinate<T,3>* res = getVertex(0);
    for (std::size_t i = 1; i < numberOfVertices(); i++) {
        if(res->pos() == getVertex(i)->pos()) {
            continue;
        }
        for (std::size_t j = 0; j < 3; j++) {
            Math::Real val1 = getVertex(i)->pos()(j);
            Math::Real val2 = res->pos()(j);
            if(Math::Util::greater(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T,3>*>
        Element<T>::getVertices() const {
    std::vector<const Coordinate::Coordinate<T,3>*> res(numberOfVertices());
    for (std::size_t i = 0; i < numberOfVertices(); i++) {
        res[i] = getVertex(i);
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T,3>*>
        Element<T>::getCoordinates() const {
    std::vector<const Coordinate::Coordinate<T,3>*> res(numberOfCoordinates());
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        res[i] = getV(i);
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T,3>*> Element<T>::getSideCoordinates(
        const std::size_t face) const {
    std::vector<const Coordinate::Coordinate<T,3>*>
        res(numberOfSideCoordinates());
    for (std::size_t i = 0; i < numberOfSideCoordinates(); i++) {
        res[i] = getSideV(face,i);
    }
    return res;
}

template<class T>
std::vector<const Coordinate::Coordinate<T,3>*> Element<T>::getSideVertices(
        const std::size_t face) const {
    std::vector<const Coordinate::Coordinate<T, 3>*> res;
    res.resize(numberOfSideVertices());
    for (std::size_t i = 0; i < numberOfSideVertices(); i++) {
        res[i] = getSideVertex(face,i);
    }
    return res;
}

template<class T>
void Element<T>::setV(const std::size_t i,
                      const Coordinate::Coordinate<T,3>* coord) {
    throw std::logic_error(
              "Setting coordinates is not allowed for this element");
}

template<class T>
bool Element<T>::vertexInCell(const Grid3& grid, const Math::Real tol) const {
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        if (!grid.isCell(*this->getV(i), tol)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Element<T>::vertexInBound() const {
    Box<T,3> bound = this->getBound();
    std::vector< Math::Vector::Cartesian<T,3> > pos = bound.getPos();
    if (pos.size() != this->numberOfCoordinates()) {
        return false;
    }
    std::vector<bool> found(pos.size(), false);
    bool foundCoord;
    for(std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        foundCoord = false;
        for(std::size_t j= 0; j < pos.size(); j++) {
            if (found[j]) {
                continue;
            }
            if (this->getV(i)->pos() == pos[j]) {
                foundCoord = true;
                found[j] = true;
                break;
            }
        }
        if (!foundCoord) {
            return false;
        }
    }
    return true;
}

template<class T>
std::vector<const CoordI3*> Element<T>::vertexToStructured(
        const CoordI3Group& cG,
        const Grid3& grid,
        const Math::Real tol) const {
    if (!this->is<ElemR>() || !this->isStructured(grid, tol)) {
        throw std::logic_error("Element::vertexToStructured unexpected empty element");
    }

    std::vector<const CoordI3*> res(this->numberOfCoordinates());
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        Math::CVecI3 cell  = grid.getCell(*this->getV(i), true, tol);
        CoordId coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            throw Error::Coord::NotFound(coordId);
        }
        res[i] = cG.getId(coordId);
        if (res[i]->pos() != cell) {
            throw Error::Coord::NotCoincident(coordId);
        }
    }

    return res;
}


template<class T>
std::vector<const CoordR3*> Element<T>::vertexToUnstructured(
        const CoordR3Group& cG,
        const Grid3& grid) const {
    if (!this->is<ElemI>()) {
        throw std::logic_error("Element::vertexToStructured unexpected empty element");
    }

    std::vector<const CoordR3*> res(this->numberOfCoordinates());
    for (std::size_t i = 0; i < this->numberOfCoordinates(); i++) {
        CoordId coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            throw Error::Coord::NotFound(coordId);
        }
        res[i] = cG.getId(coordId);
        const CoordR3* unsCoord = this->getV(i)->toUnstructured(grid);
        if (res[i]->pos() != unsCoord->pos()) {
            throw Error::Coord::NotCoincident(coordId);
        }
    }
    return res;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
