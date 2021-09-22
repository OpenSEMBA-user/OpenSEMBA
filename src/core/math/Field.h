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

#include <cstdlib>

#include "math/vector/Cartesian.h"

namespace SEMBA {
namespace Math {

template <class T, std::size_t D, std::size_t N>
class Field {
public:
    Field();
    virtual ~Field();

    T*       operator()(const std::size_t i);
    const T* operator()(const std::size_t i) const;

    Vector::Cartesian<T,D> getCVec(const std::size_t i) const;

private:
    array<T, D*N> val_;
};

} /* namespace Math */
} /* namespace SEMBA */

#include "Field.hpp"

