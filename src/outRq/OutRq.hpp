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
#include "OutRq.h"

template<class T>
OutRq<T>::OutRq(const Domain& domain,
                const Type type,
                const string& name,
                const GroupElements<T>& elems)
:   Domain(domain),
    OutRqBase(type, name),
    GroupElements<const T>(elems) {
    if (type == bulkCurrentElectric || type == bulkCurrentMagnetic) {
        if (this->getMatId(MatId(0)).size() != this->size()) {
            throw Error("Bulk currents have to be defined over elements with no material assigned.");
        }
    }
}

template<class T>
OutRq<T>::OutRq(const OutRq<T>& rhs)
:   Domain(rhs),
    OutRqBase(rhs),
    GroupElements<const T>(rhs) {

}

template<class T>
OutRq<T>::~OutRq() {

}

template<class T>
bool OutRq<T>::isSimilar(const OutRqBase& rhs) const {
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

template <class T>
void OutRq<T>::set(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::operator=(elems);
}

template <class T>
void OutRq<T>::add(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::add(elems);
}

template<class T>
inline void OutRq<T>::printInfo() const {
    cout << " --- Output request instance --- " << endl;
    OutRqBase::printInfo();
    GroupElements<const T>::printInfo();
}
