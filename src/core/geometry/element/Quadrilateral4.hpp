

#include "Quadrilateral4.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

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
     check();
}

template<class T>
Quadrilateral4<T>::Quadrilateral4(const Id id,
	std::array<const Coordinate::Coordinate<T, 3>*, 4> v,
	const Layer* lay,
	const Model* mat)
	: Identifiable<Id>(id),
	Elem(lay, mat) {

	for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
		v_[i] = v[i];
	}
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
            cG.addPos(pos[i]);
            v_[i] = cG.getPos(pos[i]);
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
std::unique_ptr<ElemI> Quadrilateral4<T>::toStructured(
        const CoordI3Group& cG,
        const Grid3& grid, const Math::Real tol) const {
    return std::make_unique<QuaI4>(this->getId(),
                            this->vertexToStructured(cG, grid, tol),
                            this->getLayer(),
                            this->getModel());
}

template<class T>
std::unique_ptr<ElemR> Quadrilateral4<T>::toUnstructured(
        const CoordR3Group& cG,
        const Grid3& grid) const {
    return std::make_unique<QuaR4>(this->getId(),
                            this->vertexToUnstructured(cG, grid),
                            this->getLayer(),
                            this->getModel());
}

template<class T>
void Quadrilateral4<T>::check() const {

}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
