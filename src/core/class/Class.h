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

#pragma once

#include <cstddef>

namespace SEMBA {
namespace Class {

class Class {
public:
    Class() {}
    virtual ~Class() {}

    template<typename T>
    bool is() const {
        if(dynamic_cast<const T*>(this) != nullptr) {
            return true;
        }
        return false;
    }

    template<typename T>
    T*       castTo() {
        return &dynamic_cast<T&>(*this);
    }
    template<typename T>
    const T* castTo() const {
        return &dynamic_cast<const T&>(*this);
    }
};

} /* namespace Class */
} /* namespace SEMBA */

