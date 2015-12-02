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

#include "Coordinate.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

Base::Base() {

}

Base::~Base() {

}

bool Base::operator ==(const Base& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

bool Base::operator !=(const Base& rhs) const {
    return !(*this == rhs);
}

template<class T, Size D>
Coordinate<T,D>::Coordinate() {

}

template<class T, Size D>
Coordinate<T,D>::Coordinate(const Id id,
                            const Math::Vector::Cartesian<T,D>& pos)
:   Identifiable<Id>(id),
    Math::Vector::Cartesian<T,D>(pos) {

}

template<class T, Size D>
Coordinate<T,D>::Coordinate(const Math::Vector::Cartesian<T,D>& pos)
:   Math::Vector::Cartesian<T,D>(pos) {

}

template<class T, Size D>
Coordinate<T,D>::Coordinate(const Coordinate& rhs)
:   Identifiable<Id>(rhs),
    Math::Vector::Cartesian<T,D>(rhs) {

}

template<class T, Size D>
Coordinate<T,D>::~Coordinate() {

}

template<class T, Size D>
Coordinate<T,D>& Coordinate<T,D>::operator=(const Coordinate& rhs) {
    if (this == &rhs)
        return *this;
    setId(rhs.getId());
    Math::Vector::Cartesian<T,D>::operator=(rhs);

    return *this;
}

template<class T, Size D>
Coordinate<T,D>* Coordinate<T,D>::clone() const {
    return new Coordinate<T,D>(*this);
}

template<class T, Size D>
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

template<class T, Size D>
bool Coordinate<T,D>::operator!=(const Base& rhs) const {
    return Base::operator!=(rhs);
}

template<class T, Size D>
bool Coordinate<T,D>::isStructured(const Grid<D>& grid,
                                   const Math::Real tol) const {
    if (!grid.isCell(*this, tol)) {
        return false;
    }
    return true;
}

template<class T, Size D>
Coordinate<Math::Int,D>* Coordinate<T,D>::toStructured(
        const Grid<D>& grid) const {
    return new Coordinate<Math::Int,D>(this->getId(), grid.getCell(*this));
}

template<class T, Size D>
Coordinate<Math::Real,D>* Coordinate<T,D>::toUnstructured(
        const Grid<D>& grid) const {
    return new Coordinate<Math::Real,D>(this->getId(), grid.getPos(*this));
}

template<class T, Size D>
void Coordinate<T,D>::printInfo() const {
    std::cout << "Id: ("<< getId() << ")  Pos: (";
    for (Size i = 0; i < D; i++) {
        std::cout << this->val[i];
        if (i < D-1) {
            std::cout << " , ";
        }
    }
    std::cout << ")" << std::endl;
}

template class Coordinate<Math::Real,3>;
template class Coordinate<Math::Int ,3>;

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
