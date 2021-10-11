

#include "Line2.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<class T>
const Math::Simplex::Line<1> Line2<T>::lin;

template<class T>
Line2<T>::Line2() {

}

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

	setCoordinates(&v[0]);
}

template<class T>
Line2<T>::Line2(Coordinate::Group<Coordinate::Coordinate<T,3> >& cG,
                const Id id,
                const Box<T,3>& box,
                const Layer* lay,
                const Model* mat)
:   Identifiable<Id>(id),
    Elem(lay, mat) {

    setCoordinates(cG, box);
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
Line2<T>::~Line2() {

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
ElemI* Line2<T>::toStructured(const Coordinate::Group<CoordI3>& cG,
                              const Grid3& grid, const Math::Real tol) const {
    const CoordI3** coords = this->vertexToStructured(cG, grid, tol);
    if (coords == nullptr) {
        return nullptr;
    }
    ElemI* res =  new LinI2(this->getId(),
                            coords,
                            this->getLayer(),
                            this->getModel());
    delete[] coords;
    return res;
}

template<class T>
ElemR* Line2<T>::toUnstructured(const Coordinate::Group<CoordR3>& cG,
                                const Grid3& grid) const {
    const CoordR3** coords = this->vertexToUnstructured(cG, grid);
    if (coords == nullptr) {
        return nullptr;
    }
    ElemR* res =  new LinR2(this->getId(),
                            coords,
                            this->getLayer(),
                            this->getModel());
    delete[] coords;
    return res;
}

template<class T>
void Line2<T>::setCoordinates(const Coordinate::Coordinate<T,3>* v[2]) {
    for (std::size_t i = 0; i < lin.np; i++) {
        v_[i] = v[i];
    }
}

template<class T>
void Line2<T>::setCoordinates(
        Coordinate::Group<Coordinate::Coordinate<T,3> >& cG,
        const Box<T,3>& box) {
    if(!box.isLine()) {
        throw Geometry::Error::Box::NotLine();
    }
    std::vector<Math::Vector::Cartesian<T,3> > pos = box.getPos();
    for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getPos(pos[i]);
        if (v_[i] == nullptr) {
            v_[i] = cG.addPos(pos[i]);
        }
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
