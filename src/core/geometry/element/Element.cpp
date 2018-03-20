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

#include "Element.h"

#include <algorithm>

namespace SEMBA {
namespace Geometry {
namespace Element {

Base::Base(const Layer* lay,
           const Model* mat) {
    lay_ = lay;
    mat_ = mat;
}

Base::Base(const Base& rhs) {
    lay_ = rhs.lay_;
    mat_ = rhs.mat_;
}

Base::~Base() {

}

bool Base::operator==(const Base& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

LayerId Base::getLayerId() const {
    if (lay_ == nullptr) {
        return LayerId(0);
    }
    return lay_->getId();
}

MatId Base::getMatId  () const {
    if (mat_ == nullptr) {
        return MatId(0);
    }
    return mat_->getId();
}

bool Base::operator!=(const Base& rhs) const {
    return !(*this == rhs);
}

std::vector<CoordId> Base::ascendingIdOrder(
        const std::vector<CoordId>& in) {
    std::vector<CoordId> res = in;
    std::sort(res.begin(), res.end());
    return res;
}

void Base::printInfo() const {
    std::cout << "Element. Id: " << this->getId()
              << " MatId: " << this->getMatId()
              << " LayerId: " << this->getLayerId() << std::endl;
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
