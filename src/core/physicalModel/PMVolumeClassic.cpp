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
 * PMVolumeClassic.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#include "PMVolumeClassic.h"

PMVolumeClassic::PMVolumeClassic(
        const MatId matId,
        const string& name,
        const Real relativePermittivity,
        const Real relativePermeability,
        const Real electricConductivity,
        const Real magneticConductivity) : PMVolume(matId, name) {
    rEps_ = relativePermittivity;
    rMu_ = relativePermeability;
    electricConductivity_ = electricConductivity;
    magneticConudctivity_ = magneticConductivity;
}

PMVolumeClassic::~PMVolumeClassic() {
}

Real PMVolumeClassic::getImpedance() const {
    if (rEps_ <= 0.0) {
        return numeric_limits<Real>::infinity();
    }
    return sqrt((rMu_ * Constants::mu0) / (rEps_ * Constants::eps0));
}

Real PMVolumeClassic::getAdmitance() const {
    if (rMu_ <= 0.0) {
        return numeric_limits<Real>::infinity();
    }
    return (1.0 / getImpedance());
}

Real PMVolumeClassic::getRelativePermittivity() const {
    return rEps_;
}

Real PMVolumeClassic::getRelativePermeability() const {
    return rMu_;
}

Real PMVolumeClassic::getPermittivity() const {
    return (rEps_ * Constants::eps0);
}

Real PMVolumeClassic::getPermeability() const {
    return (rMu_ * Constants::mu0);
}

Real PMVolumeClassic::getElectricConductivity() const {
    return electricConductivity_;
}

Real PMVolumeClassic::getMagneticConductivity() const {
    return magneticConudctivity_;
}

bool PMVolumeClassic::isVacuum() const {
    return (rEps_ == 1.0
            && rMu_ == 1.0
            && electricConductivity_ == 0.0
            && magneticConudctivity_ == 0.0);
}

