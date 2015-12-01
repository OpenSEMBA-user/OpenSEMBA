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

#include "MultiportRLC.h"

namespace SEMBA {
namespace PhysicalModel {

MultiportRLC::MultiportRLC(
        const Id id,
        const std::string name,
        const Multiport::Type type,
        const Math::Real resistance,
        const Math::Real inductance,
        const Math::Real capacitance)
:   Multiport(id, name) {
    type_ = type;
    R_ = resistance;
    L_ = inductance;
    C_ = capacitance;
}

MultiportRLC::~MultiportRLC() {

}

Math::Real MultiportRLC::getR() const {
    return R_;
}

Math::Real MultiportRLC::getL() const {
    return L_;
}

Math::Real MultiportRLC::getC() const {
    return C_;
}

void MultiportRLC::printInfo() const {
    std::cout<< " --- Multiport RLC Info --- " << std::endl;
    Multiport::printInfo();
    std::cout<< " Type: " << getTypeStr() << std::endl;
    std::cout<< " Resistance: " << R_ << std::endl;
    std::cout<< " Inductance: " << L_ << std::endl;
    std::cout<< " Capacitance: " << C_ << std::endl;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
