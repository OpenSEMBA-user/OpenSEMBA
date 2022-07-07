

#include "Line2.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
const Math::Simplex::Line<1> Line2<T>::lin;

template<class T>
Line2<T>::Line2(const Id id,
                const Coordinate::Coordinate<T,3>* v[2],
                const Layer* lay,
                const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {
    
    setCoordinates(v);
}

template<class T>
Line2<T>::Line2(const Id id,
	std::array<const Coordinate::Coordinate<T, 3>*,2> v,
	const Layer* lay,
	const Model* mat)
	: Identifiable<Id>(id),
	Elem(lay, mat) {

    // TODO: Remove plain array
	setCoordinates(&v[0]);
}


template<class T>
Line2<T>::Line2(const Coordinate::Coordinate<T,3>* v[2]) {
    setCoordinates(v);
}

template<class T>
Line2<T>::Line2(Coordinate::Group<Coordinate::Coordinate<T,3> >& cG,
                const Box<T,3>& box) {

    setCoordinates(cG, box);
}


template<class T>
Line2<T>::Line2(const Line2<T>& rhs)
:   Identifiable<Id>(rhs),
    Elem(rhs) {
    
    for (std::size_t i = 0; i < lin.np; i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
bool Line2<T>::isStructured(const Grid3& grid, const Math::Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isLine()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
}

template<class T>
const Coordinate::Coordinate<T,3>* Line2<T>::getV(const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Line2<T>::getSideV(
        const std::size_t f,
        const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Line2<T>::getVertex(
        const std::size_t i) const {
    return v_[i];
}

template<class T>
const Coordinate::Coordinate<T,3>* Line2<T>::getSideVertex(
        const std::size_t f,
        const std::size_t i) const {
    return v_[i];
}

template<class T>
void Line2<T>::setV(const std::size_t i,
                    const Coordinate::Coordinate<T,3>* coord) {

    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
void Line2<T>::setCoordinates(const Coordinate::Coordinate<T,3>* v[2]) {
    for (std::size_t i = 0; i < lin.np; i++) {
        v_[i] = v[i];
    }
}

template<class T>
std::unique_ptr<ElemI> Line2<T>::toStructured(
    const CoordI3Group& cG,
    const Grid3& grid, const Math::Real tol) const {
    return std::make_unique<LinI2>(this->getId(),
        this->vertexToStructured(cG, grid, tol).data(),
        this->getLayer(),
        this->getModel()
    );
}

template<class T>
std::unique_ptr<ElemR> Line2<T>::toUnstructured(
    const CoordR3Group& cG,
    const Grid3& grid) const {
    return std::make_unique<LinR2>(this->getId(),
        this->vertexToUnstructured(cG, grid).data(),
        this->getLayer(),
        this->getModel()
    );
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
