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

#include "Relative.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

Relative::Relative() {

}

Relative::Relative(const Id id,
                   const Math::CVecR3& rel)
:   Identifiable<Id>(id) {
    for (std::size_t d = 0; d < 3; d++) {
        this->pos()(d) = (int)std::floor(rel(d));
        this->rel_ (d) = rel(d) - this->pos()(d);
    }
}

Relative::Relative(const Id id,
                   const Math::CVecI3& pos,
                   const Math::CVecR3& rel)
:   Identifiable<Id>(id),
    Math::CVecI3(pos) {
    rel_ = rel;
}

Relative::Relative(const Relative& rhs)
:   Identifiable<Id>(rhs),
    Math::CVecI3(rhs) {
    rel_ = rhs.rel_;
}

Relative::~Relative() {

}

Relative& Relative::operator=(const Relative& rhs) {
    if (this == &rhs)
        return *this;

    CoordI3::operator=(rhs);
    rel_ = rhs.rel_;
    return *this;
}

bool Relative::operator==(const Base& rhs) const {
    if (!Coordinate<Math::Int,3>::operator==(rhs)) {
        return false;
    }
    const Relative* rhsPtr = rhs.castTo<Relative>();
    bool res = true;
    res &= (this->rel_ == rhsPtr->rel_);
    return res;
}

CoordR3* Relative::toUnstructured(const Grid3& grid) const {
    Math::CVecR3 pos = grid.getPos(*this);
    for (std::size_t d = 0; d < 3; d++) {
        Math::Real length = rel_(d);
        Math::Int cellDir = this->pos()(d) + 1;
        Math::Real posAux = grid.getPos(d, cellDir);
        Math::Real step = posAux - pos(d);
        pos(d) += step * length;
    }
    return new CoordR3(this->getId(), pos);
}

void Relative::printInfo() const {
    CoordI3::printInfo();
    std::cout << " Relative: ";
    rel_.printInfo();
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
