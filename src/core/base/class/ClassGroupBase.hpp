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
 * ClassGroup.hpp
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#include "ClassGroupBase.h"

template<class T>
ClassGroupBase<T>::ErrorEmpty::ErrorEmpty(const std::string& func)
:   Error(func + std::string(" has no elements")) {

}

template<class T>
ClassGroupBase<T>::ErrorEmpty::~ErrorEmpty() throw() {

}

template<typename T>
ClassGroupBase<T>::ClassGroupBase() {

}

template<typename T>
ClassGroupBase<T>::~ClassGroupBase() {

}

template<class T>
bool ClassGroupBase<T>::check(const std::string& func) const {
    if (this->elems().empty()) {
        throw ErrorEmpty(func);
    }
    return true;
}
