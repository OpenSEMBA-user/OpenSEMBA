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

#include "Conformal.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

Conformal::Conformal() {
    dir_ = Math::Constants::x;
    length_ = 0.0;
}

Conformal::Conformal(const Id id,
                     const Math::CVecI3& pos,
                     const Math::Constants::CartesianAxis dir,
                     const Math::Real length)
:   Identifiable<Id>(id),
    Math::CVecI3(pos) {

    dir_    = dir;
    length_ = length;
}

Conformal::Conformal(const Math::Constants::CartesianAxis dir,
                     const Math::Real length) {

    dir_    = dir;
    length_ = length;
}

Conformal::Conformal(const Conformal& rhs)
:   Identifiable<Id>(rhs),
    Math::CVecI3(rhs) {

    dir_    = rhs.dir_;
    length_ = rhs.length_;
}

Conformal::~Conformal() {

}

Conformal& Conformal::operator=(const Conformal& rhs) {
    if (this == &rhs)
        return *this;

    CoordI3::operator=(rhs);
    dir_    = rhs.dir_;
    length_ = rhs.length_;

    return *this;
}

bool Conformal::operator==(const Base& rhs) const {
    if (!Coordinate<Math::Int,3>::operator==(rhs)) {
        return false;
    }
    const Conformal* rhsPtr = rhs.castTo<Conformal>();
    bool res = true;
    res &= (this->length_ == rhsPtr->length_);
    res &= (this->dir_ == rhsPtr->dir_);
    return res;
}

CoordR3* Conformal::toUnstructured(const Grid3& grid) const {
    Math::CVecR3 pos = grid.getPos(*this);
    if (Math::Util::greater(getLength(), 0.0)) {
        Math::Int dir = getDir();
        Math::Real length = getLength();
        Math::CVecI3 cellAux = *this;
        cellAux(dir)++;
        Math::CVecR3 posAux = grid.getPos(cellAux);
        Math::Real step = posAux(dir)-pos(dir);
        pos(dir) += step*length;
    }
    return new CoordR3(this->getId(), pos);
}

void Conformal::printInfo() const {
    CoordI3::printInfo();
    std::cout << " Dir: (";
    if(Math::Util::equal(length_, 0.0)) {
        std::cout << "0";
    } else {
        std::cout << ('x'+dir_);
    }
    std::cout << ")";
    if(Math::Util::notEqual(length_, 0.0)) {
        std::cout << " Length: (" << length_ << ")";
    }
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
