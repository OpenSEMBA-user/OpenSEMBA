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

#include "VolumePML.h"

namespace SEMBA {
namespace PhysicalModel {

VolumePML::VolumePML(const Id id,
                     const std::string& name,
                     const Math::Axis::Local* orientation)
:   Identifiable<Id>(id),
    PhysicalModel(name) {
    orientation_ = orientation;
}

VolumePML::VolumePML(const VolumePML& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    if (rhs.orientation_ != NULL) {
        orientation_ = new Math::Axis::Local(*rhs.orientation_);
    } else {
        orientation_ = NULL;
    }
}

VolumePML::~VolumePML() {
    if (orientation_ != NULL) {
        delete orientation_;
    }
}

void VolumePML::printInfo() const {
    std::cout << "--- VolumePML info ---" << std::endl;
    Volume::printInfo();
    if (orientation_ != NULL) {
        orientation_->printInfo();
    }

}

const Math::Axis::Local* VolumePML::getOrientation() const {
    return orientation_;
}
} /* namespace PhysicalModel */
} /* namespace SEMBA */
