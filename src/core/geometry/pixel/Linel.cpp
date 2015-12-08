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

#include "Linel.h"

namespace SEMBA {
namespace Geometry {
namespace Pixel {

Linel::Linel() {
    dirId_ = Math::CartesianDirection::dirNode;
}

Linel::~Linel() {
}

bool Linel::isInto (const Math::CVecI3Fractional& fracPos)const{
    Math::CVecR3 pos = fracPos.getScalePos(*this);
    for(std::size_t n=0; n<3; n++){
        if(n==(std::size_t)(dirId_-1)){
            if (pos(n)<0.0 || pos(n)>1.0 ){
                return false;
            }
        }else if (!Math::Util::equal(pos(n),0.0)){
            return false;
        }
    }
    return true;
}

Linel::Linel(const Math::CVecI3& pos, const Math::CartesianDirection& dir) :
    Pixel(pos) {
    dirId_ = dir;
}

bool Linel::operator ==(const Linel& rhs) const {
    return (Pixel::operator==(rhs) && rhs.dirId_ == dirId_);
}

std::string Linel::toStr() const {
    std::stringstream res;
    res << Math::CVecI3::toStr() << " Dir: " << Pixel::toStr(dirId_);
    return res.str();
}

bool Linel::operator<(const Linel& rhs) const {
    if (Pixel::operator<(rhs)) {
        return true;
    }
    if (Pixel::operator==(rhs)) {
        return dirId_ < rhs.dirId_;
    }
    return false;
}

} /* namespace Pixel */
} /* namespace Geometry */
} /* namespace SEMBA */
