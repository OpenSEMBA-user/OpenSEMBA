/*
 * PMVolumeAnistropicFerrite.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#include "PMVolumeAnistropicFerrite.h"

PMVolumeAnistropicFerrite::PMVolumeAnistropicFerrite(
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

PMVolumeAnistropicFerrite::~PMVolumeAnistropicFerrite() {

}

