/*
 * PMVolumeAnisotropic.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#include "PMVolumeAnisotropic.h"

PMVolumeAnisotropic::PMVolumeAnisotropic(
        const MatId matId,
        const string& name,
        const LocalAxes& localAxe) : PMVolume(matId, name) {
    localAxe_ = localAxe;
}

PMVolumeAnisotropic::~PMVolumeAnisotropic() {

}

LocalAxes PMVolumeAnisotropic::getLocalAxe() const {
    return localAxe_;
}
