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
 * EMSource.hpp
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#include "../sources/EMSource.h"

template <class T>
void EMSource<T>::set(
        const GroupElements<const Elem>& elems) {
    GroupElements<const T>::operator=(elems);
}

template <class T>
void EMSource<T>::add(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::add(elems);
}

template<class T>
bool EMSource<T>::isSimilar(const EMSourceBase& rhs) const {
    if (!this->hasSameProperties(rhs)) {
        return false;
    }
    UInt sizeLhs = this->elems().size();
    UInt sizeRhs = rhs.elems().size();
    if ((sizeLhs == 0) && (sizeRhs == 0)) {
        return true;
    } else if ((sizeLhs == 0) || (sizeRhs == 0)) {
        return false;
    }
    return true;
}

template<class T>
void EMSource<T>::printInfo() const {
    EMSourceBase::printInfo();
    cout << "Size of element group: "
            << this->GroupElements<const T>::size() << endl;
//    this->GroupElements<const T>::printInfo();
}
