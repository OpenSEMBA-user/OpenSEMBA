/*
 * PortTEMCoaxial.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#include "PortTEMCoaxial.h"

PortTEMCoaxial::PortTEMCoaxial() {
    innerRadius_ = 0.0;
    outerRadius_ = 0.0;
}

PortTEMCoaxial::~PortTEMCoaxial() {
}

CVecR3 PortTEMCoaxial::getOrigin(const BoundTerminations& sym) const {
    return origin_;
}

CVecR3 PortTEMCoaxial::getWeight(
        const CVecR3& pos,
        const BoundTerminations& sym) const {
    // Return normalized weights for electric field components.
    const Real rho = (pos - getOrigin()).norm();
    switch (getExcitationMode()) {
    case ExcitationMode::voltage:
        const CVecR3 rhoHat = (pos - getOrigin()).normalize();
        return rhoHat / (rho * log(outerRadius_/innerRadius_));
    case ExcitationMode::current:
        const CVecR3 phiHat = (CVecR3(0,0,1) ^ pos).normalize();
        return phiHat / (2*M_PI*rho);
    }
}
