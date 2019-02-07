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

#include "PML.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Bound {

PML::PML(const Id id)
:   Identifiable<Id>(id),
    PhysicalModel("PML") {

}

PML::PML(const PML& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {

}

PML::~PML() {

}

void PML::printInfo() const {
    std::cout << "--- PML info ---" << std::endl;
    std::cout << "Perfectly Matched Layer" << std::endl;
}

} /* namespace Bound */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
