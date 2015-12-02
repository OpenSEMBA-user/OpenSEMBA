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

#include "TEM.h"

namespace SEMBA {
namespace Source {
namespace Port {

TEM::~TEM() {

}

TEM::TEM(
        Magnitude::Magnitude* magnitude,
        const Geometry::Element::Group<const Geometry::Surf>& elem,
        const ExcitationMode excitationMode) : Port(magnitude, elem) {
    excitationMode_ = excitationMode;
}

TEM::TEM(const TEM& rhs) : Port(rhs) {
    excitationMode_ = rhs.excitationMode_;
}

TEM::ExcitationMode TEM::getExcitationMode() const {
    return excitationMode_;
}

void TEM::printInfo() const {
    std::cout << " --- TEM Port --- " << std::endl;
    std::cout << "Excitation mode: " << toStr(excitationMode_) << std::endl;
    Port::printInfo();
}

std::string TEM::toStr(const ExcitationMode& excitationMode) {
    switch (excitationMode) {
    case ExcitationMode::voltage:
        return std::string("voltage");
    case ExcitationMode::current:
    default:
        return std::string("current");
    }
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
