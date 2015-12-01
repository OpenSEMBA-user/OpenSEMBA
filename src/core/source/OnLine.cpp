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

#include "OnLine.h"

namespace SEMBA {
namespace Source {

OnLine::OnLine(const Magnitude::Magnitude* magnitude,
               const Geometry::Element::Group<Geometry::Lin>& elem,
               const Type& sourceType,
               const Hardness& sourceHardness)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Lin>(elem) {

    type_ = sourceType;
    hardness_ = sourceHardness;
}

OnLine::OnLine(const OnLine& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Lin>(rhs) {

    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
}

OnLine::~OnLine() {

}

bool OnLine::hasSameProperties(const SEMBA::Source::Base& rhs) const {
    if(!SEMBA::Source::Base::hasSameProperties(rhs)) {
        return false;
    }
    const OnLine* rhsPtr = rhs.castTo<OnLine>();
    bool hasSameProperties = true;
    hasSameProperties &= type_ == rhsPtr->type_;
    hasSameProperties &= hardness_ == rhsPtr->hardness_;
    return hasSameProperties;
}

const std::string& OnLine::getName() const {
    const static std::string res = "OnLine";
    return res;
}

OnLine::Type OnLine::getType() const {
    return type_;
}

OnLine::Hardness OnLine::getHardness() const {
    return hardness_;
}

void OnLine::printInfo() const {
    std::cout<< " --- Nodal info --- " << std::endl;
    SEMBA::Source::Base::printInfo();
    std::cout<< "Type: " << getTypeStr() << std::endl;
    std::cout<< "Hardness: " << getHardnessStr() << std::endl;
}

std::string OnLine::getTypeStr() const {
    switch (type_) {
    case electric:
        return "Electric Field";
    default:
        return "Magnetic Field";
    }
}

std::string OnLine::getHardnessStr() const {
    switch (type_) {
    case hard:
        return "Hard";
    default:
        return "Soft";
    }
}

} /* namespace Source */
} /* namespace SEMBA */
