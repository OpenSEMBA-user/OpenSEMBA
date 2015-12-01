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
 * PMVolumeAnisotropicCrystal.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#include "PMVolumeAnisotropicCrystal.h"


PMVolumeAnisotropicCrystal::PMVolumeAnisotropicCrystal(
        const MatId matId,
        const string& name,
        const LocalAxes& local,
        const CVecR3& principalAxesRelativePermittivity,
        const Real relativePermeability)
: PMVolumeAnisotropic(matId, name, local) {
    principalAxesRelativePermittivity_ = principalAxesRelativePermittivity;
    relativePermeability_ = relativePermeability;
}

PMVolumeAnisotropicCrystal::~PMVolumeAnisotropicCrystal() {

}

const CVecR3 PMVolumeAnisotropicCrystal::getPrincipalAxesRelativePermittivity() const {
    return principalAxesRelativePermittivity_;
}

Real PMVolumeAnisotropicCrystal::getRelativePermeability() const {
    return relativePermeability_;
}


MatR33 PMVolumeAnisotropicCrystal::getRelPermittivityMatR() const {
    MatR33 local;
    local.setInDiagonal(principalAxesRelativePermittivity_);
    return getLocalAxe().convertToGlobal(local);
}

MatR33 PMVolumeAnisotropicCrystal::getRelPermeabilityMatR() const {
    MatR33 local;
    local.setInDiagonal(CVecR3(relativePermeability_));
    return getLocalAxe().convertToGlobal(local);
}

MatR33 PMVolumeAnisotropicCrystal::getElectricConductivityMat() const {
    return MatR33();
}

MatR33 PMVolumeAnisotropicCrystal::getMagneticConductivityMat() const {
    return MatR33();
}
