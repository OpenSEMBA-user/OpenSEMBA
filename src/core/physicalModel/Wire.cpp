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

#include "Wire.h"

namespace SEMBA {
namespace PhysicalModel {

Wire::Wire(const Id id,
           const std::string name,
           const Math::Real radius,
           const Math::Real resistance,
           const Math::Real inductance)
:   PhysicalModel(id, name) {
    radius_ = radius;
    resistance_ = resistance;
    inductance_ = inductance;
}

Wire::~Wire() {
}

Math::Real Wire::getRadius() const {
    return radius_;
}

Math::Real Wire::getResistance() const {
    return resistance_;
}

Math::Real Wire::getInductance() const {
    return inductance_;
}

void Wire::printInfo() const {
    std::cout<< " --- Wire info ---" << std::endl;
    PhysicalModel::printInfo();
    std::cout << " Radius: " << radius_ << std::endl
              << " Resistance: " << resistance_ << std::endl
              << " Inductance: " << inductance_ << std::endl;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
