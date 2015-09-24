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
/*
 * Waveport.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "PortWaveguide.h"

PortWaveguide::PortWaveguide(Magnitude* magnitude,
        const GroupElements<const Surf>& elem,
        const ExcitationMode excMode,
        const pair<UInt,UInt> mode)
:   Port(magnitude, elem) {

    excitationMode_ = excMode;
    mode_ = mode;
    // Performs checks
    if (!this->getBound().isSurface()) {
        throw Error("Waveport elements must be contained in a coplanar surface");
    }

    CVecR3 diagonal = this->getBound().getMax() - this->getBound().getMin();
    if (!diagonal.isContainedInPlane(xy)) {
        throw Error("Waveport must be contained in plane xy.");
    }

    if (this->size() == 0) {
        throw Error("Waveport must contain some elements.");
    }


    check();
}

PortWaveguide::PortWaveguide(const PortWaveguide& rhs) :
                Port(rhs) {
    excitationMode_ = rhs.excitationMode_;
    mode_ = rhs.mode_;
}

PortWaveguide::~PortWaveguide() {

}

bool PortWaveguide::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const PortWaveguide* rhsPtr = rhs.castTo<PortWaveguide>();
    bool hasSameProperties = true;
    hasSameProperties &= mode_ == rhsPtr->mode_;
    hasSameProperties &= excitationMode_ == rhsPtr->excitationMode_;
    return hasSameProperties;
}

PortWaveguide::ExcitationMode PortWaveguide::getExcitationMode() const {
    return excitationMode_;
}

pair<UInt, UInt> PortWaveguide::getMode() const {
    return mode_;
}

void PortWaveguide::printInfo() const {
    cout << "--- Waveguide port info ---" << endl;
    cout << "Mode: ";
    if (excitationMode_ == TE) {
        cout << "TE ";
    } else {
        cout << "TM ";
    }
    cout << mode_.first << ", " << mode_.second << endl;
    Port::printInfo();
}
