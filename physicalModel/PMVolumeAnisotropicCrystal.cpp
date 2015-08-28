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
