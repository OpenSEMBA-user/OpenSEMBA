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
 * Vector.hpp
 *
 *  Created on: 23/6/2015
 *      Author: Daniel
 */

#include "Vector.h"

template<typename T>
Vector<T>::Vector() {

}

template<typename T>
Vector<T>::Vector(const T& elems) {
    element_.reserve(1);
    element_.push_back(elems);
}

template<typename T>
Vector<T>::Vector(const std::vector<T>& elems) {
    element_ = elems;
}

template<typename T>
Vector<T>::Vector(const Vector& rhs) {
    element_ = rhs.element_;
}

template<typename T>
Vector<T>::~Vector() {

}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
    if (this == &rhs) {
        return *this;
    }
    element_ = rhs.element_;
    return *this;
}

template<typename T>
typename Reference<T>::type Vector<T>::get(const UInt i) {
    return element_[i];
}

template<typename T>
typename ConstReference<T>::type Vector<T>::get(const UInt i) const {
    return element_[i];
}

template<typename T>
Vector<T> Vector<T>::add(const Vector& rhs) {
    element_.insert(element_.end(), rhs.element_.begin(), rhs.element_.end());
    return rhs;
}
