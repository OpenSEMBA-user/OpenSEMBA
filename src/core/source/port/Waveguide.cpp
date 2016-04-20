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

#include "Waveguide.h"

namespace SEMBA {
namespace Source {
namespace Port {

Waveguide::Waveguide(Magnitude::Magnitude* magnitude,
        const Geometry::Element::Group<const Geometry::Surf>& elem,
        const ExcitationMode excMode,
        const std::pair<size_t,size_t> mode)
:   Port(magnitude, elem) {

    excitationMode_ = excMode;
    mode_ = mode;
    // Performs checks
    if (!this->getBound().isSurface()) {
        throw std::logic_error("Waveport elements must be contained "
                               "in a coplanar Geometry::Surface");
    }

    Math::CVecR3 diagonal = this->getBound().getMax() - 
                            this->getBound().getMin();
    if (!diagonal.isContainedInPlane(Math::Constants::xy)) {
        throw std::logic_error("Waveport must be contained in plane xy.");
    }

    if (this->size() == 0) {
        throw std::logic_error("Waveport must contain some elements.");
    }
}

Waveguide::Waveguide(const Waveguide& rhs) :
                Port(rhs) {
    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

Waveguide::~Waveguide() {

}

bool Waveguide::hasSameProperties(const SEMBA::Source::Base& rhs) const {
    if(!SEMBA::Source::Base::hasSameProperties(rhs)) {
        return false;
    }
    const Waveguide* rhsPtr = rhs.castTo<Waveguide>();
    bool hasSameProperties = true;
    hasSameProperties &= mode_ == rhsPtr->mode_;
    hasSameProperties &= excitationMode_ == rhsPtr->excitationMode_;
    return hasSameProperties;
}

Waveguide::ExcitationMode Waveguide::getExcitationMode() const {
    return excitationMode_;
}

std::pair<size_t,size_t> Waveguide::getMode() const {
    return mode_;
}

void Waveguide::printInfo() const {
    std::cout << "--- Waveguide port info ---" << std::endl;
    std::cout << "Mode: ";
    if (excitationMode_ == TE) {
        std::cout << "TE ";
    } else {
        std::cout << "TM ";
    }
    std::cout << mode_.first << ", " << mode_.second << std::endl;
    Port::printInfo();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
