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
 * PMVolumeAnistropicFerrite.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#include "PMVolumeAnisotropicFerrite.h"

PMVolumeAnisotropicFerrite::PMVolumeAnisotropicFerrite(
        const MatId matId,
        const string& name,
        const LocalAxes& local,
        const Real kappa,
        const Real relativePermeability,
        const Real relativePermittivity)
: PMVolumeAnisotropic(matId, name, local) {
    kappa_ = kappa;
    relativePermeability_ = relativePermeability;
    relativePermittivity_ = relativePermittivity;
}

PMVolumeAnisotropicFerrite::~PMVolumeAnisotropicFerrite() {

}

MatR33 PMVolumeAnisotropicFerrite::getRelPermittivityMatR() const {
    return MatR33().setInDiagonal(CVecR3(relativePermittivity_));
}

MatR33 PMVolumeAnisotropicFerrite::getRelPermeabilityMatR() const {
    MatR33 local;
    CVecR3 principalAxis(relativePermeability_, relativePermeability_, 1.0);
    local.setInDiagonal(principalAxis);
    return getLocalAxe().convertToGlobal(local);
}

MatR33 PMVolumeAnisotropicFerrite::getRelPermeabilityMatI() const {
    MatR33 local;
    local(0,1) =   kappa_;
    local(1,0) = - kappa_;
    return getLocalAxe().convertToGlobal(local);
}

MatR33 PMVolumeAnisotropicFerrite::getElectricConductivityMat() const {
    return MatR33();
}

MatR33 PMVolumeAnisotropicFerrite::getMagneticConductivityMat() const {
    return MatR33();
}
