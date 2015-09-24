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
/*
 * Array.hpp
 *
 *  Created on: 24/6/2015
 *      Author: Daniel
 */

#include "Array.h"

template<typename T, Int D>
Array<T,D>::Array() {

}

template<typename T, Int D>
Array<T,D>::Array(const T& elem) {
    for (Int d = 0; d < D; d++) {
        element_[d] = elem;
    }
}

template<typename T, Int D>
Array<T,D>::Array(const std::array<T,D>& elems) {
    element_ = elems;
}

template<typename T, Int D>
Array<T,D>::Array(const Array& rhs) {
    element_ = rhs.element_;
}

template<typename T, Int D>
Array<T,D>::~Array() {

}

template<typename T, Int D>
Array<T,D>& Array<T,D>::operator=(const Array& rhs) {
    if (this == &rhs) {
        return *this;
    }
    element_ = rhs.element_;
    return *this;
}

template<typename T, Int D>
typename Reference<T>::type Array<T,D>::get(const UInt i) {
    return element_[i];
}

template<typename T, Int D>
typename ConstReference<T>::type Array<T,D>::get(const UInt i) const {
    return element_[i];
}
