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

#include "Cloneable.h"

namespace SEMBA {
namespace Group {

template<typename T>
Group<T>* Cloneable<T>::clone() const {
    return new Cloneable<T>(this->cloneElems());
}

template<typename T>
Group<typename std::remove_const<T>::type> Cloneable<T>::cloneElems() const {
    Group<typename std::remove_const<T>::type> res;
    res.reserve(this->size());
    for (std::size_t i = 0; i < this->size(); i++) {
        res.add(
            dynamic_cast<
                typename std::add_pointer<
                             typename std::remove_const<T>::type>::type>(
                                 this->get(i)->clone()));
    }
    return res;
}

} /* namespace Group */
} /* namespace SEMBA */
