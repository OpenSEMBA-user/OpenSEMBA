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

#include "Source.h"

namespace SEMBA {
namespace Source {

template <class T>
void Source<T>::set(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::operator=(elems);
}

template <class T>
void Source<T>::add(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::add(elems);
}

template<class T>
bool Source<T>::isSimilar(const SEMBA::Source::Base& rhs) const {
    if (!this->hasSameProperties(rhs)) {
        return false;
    }
    std::size_t sizeLhs = this->elems().size();
    std::size_t sizeRhs = rhs.elems().size();
    if ((sizeLhs == 0) && (sizeRhs == 0)) {
        return true;
    } else if ((sizeLhs == 0) || (sizeRhs == 0)) {
        return false;
    }
    return true;
}

template<class T>
void Source<T>::printInfo() const {
    Base::printInfo();
    std::cout << "std::size_t of element group: "
              << this->Geometry::Element::Group<const T>::size() << std::endl;
//    this->GroupElements<const T>::printInfo();
}

} /* namespace Source */
} /* namespace SEMBA */
