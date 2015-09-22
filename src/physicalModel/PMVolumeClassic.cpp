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

