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

#include <physicalModel/wire/Extremes.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Wire {

Extremes::Extremes(const std::string& name,
                   const Wire& wire,
                   const Multiport::Multiport* extremeL,
                   const Multiport::Multiport* extremeR)
:   Identifiable<Id>(wire),
    PhysicalModel(name),
    Wire(wire) {
    extreme_[0] = extreme_[1] = NULL;
    if (extremeL != NULL) {
        extreme_[0] = extremeL->cloneTo<Multiport::Multiport>();
    }
    if (extremeR != NULL) {
        extreme_[1] = extremeR->cloneTo<Multiport::Multiport>();
    }
}

Extremes::Extremes(const Extremes& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs),
    Wire(rhs) {
    extreme_[0] = extreme_[1] = NULL;
    if (rhs.extreme_[0] != NULL) {
        extreme_[0] = rhs.extreme_[0]->cloneTo<Multiport::Multiport>();
    }
    if (rhs.extreme_[1] != NULL) {
        extreme_[1] = rhs.extreme_[1]->cloneTo<Multiport::Multiport>();
    }
}

Extremes::~Extremes() {
    if (extreme_[0] != NULL) {
        delete extreme_[0];
    }
    if (extreme_[1] != NULL) {
        delete extreme_[1];
    }
}

void Extremes::setExtreme(const std::size_t i,
                          const Multiport::Multiport* extreme) {
    if (extreme_[i] != NULL) {
        delete extreme_[i];
    }
    extreme_[i] = NULL;
    if (extreme != NULL) {
        extreme_[i] = extreme->cloneTo<Multiport::Multiport>();
    }
}

void Extremes::swapExtremes() {
    std::swap(extreme_[0], extreme_[1]);
}

void Extremes::printInfo() const {
    std::cout<< " --- WireExtremes info ---" << std::endl;
    Wire::printInfo();
    if (extreme_[0] != NULL) {
        std::cout << "ExtremeL:" << std::endl;
        extreme_[0]->printInfo();
    }
    if (extreme_[1] != NULL) {
        std::cout << "ExtremeR:" << std::endl;
        extreme_[1]->printInfo();
    }
}

} /* namespace Wire */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
