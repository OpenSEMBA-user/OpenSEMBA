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
