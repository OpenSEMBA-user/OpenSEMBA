#include "Coordinate.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

template<class T, std::size_t D>
Coordinate<T,D>::Coordinate(const Id id,
                            const Math::Vector::Cartesian<T,D>& pos)
:   Identifiable<Id>(id),
    Math::Vector::Cartesian<T,D>(pos) {

}

template<class T, std::size_t D>
Coordinate<T,D>::Coordinate(const Math::Vector::Cartesian<T,D>& pos)
:   Math::Vector::Cartesian<T,D>(pos) {

}

template<class T, std::size_t D>
Coordinate<T,D>::Coordinate(const Coordinate& rhs)
:   Identifiable<Id>(rhs),
    Math::Vector::Cartesian<T,D>(rhs) {

}

template<class T, std::size_t D>
Coordinate<T,D>& Coordinate<T,D>::operator=(const Coordinate& rhs) 
{
    setId(rhs.getId());
    Math::Vector::Cartesian<T,D>::operator=(rhs);
    return *this;
}

template<class T, std::size_t D>
bool Coordinate<T,D>::operator==(const Base& rhs) const {
    if(!Base::operator==(rhs)) {
        return false;
    }
    const Coordinate<T,D>* rhsPtr = rhs.castTo<Coordinate<T,D> >();
    bool res = true;
    res &= (this->getId() == rhsPtr->getId());
    res &= (this->pos() == rhsPtr->pos());
    return res;
}

template<class T, std::size_t D>
bool Coordinate<T,D>::operator!=(const Base& rhs) const {
    return Base::operator!=(rhs);
}

template<class T, std::size_t D>
bool Coordinate<T,D>::isStructured(const Grid<D>& grid,
                                   const Math::Real tol) const {
    if (!grid.isCell(*this, tol)) {
        return false;
    }
    return true;
}

template<class T, std::size_t D>
Coordinate<Math::Int,D>* Coordinate<T,D>::toStructured(
        const Grid<D>& grid) const {
    return new Coordinate<Math::Int,D>(this->getId(), grid.getCell(*this));
}

template<class T, std::size_t D>
Coordinate<Math::Real,D>* Coordinate<T,D>::toUnstructured(
        const Grid<D>& grid) const {
    return new Coordinate<Math::Real,D>(this->getId(), grid.getPos(*this));
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
