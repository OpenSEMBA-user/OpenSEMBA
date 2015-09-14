/*
 * PortTEMCoaxial.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: luis
 */

#include "PortTEMCoaxial.h"

PortTEMCoaxial::PortTEMCoaxial(
        Magnitude* magnitude,
        const GroupElements<const Surf>& elem,
        const ExcitationMode excMode,
        const CVecR3& origin,
        const Real innerRadius,
        const Real outerRadius) :
        PortTEM(magnitude, elem, excMode) {
    origin_ = origin;
    innerRadius_ = innerRadius;
    outerRadius_ = outerRadius;
}

PortTEMCoaxial::PortTEMCoaxial(
        const PortTEMCoaxial& rhs) :
        PortTEM(rhs) {
    origin_ = rhs.origin_;
    innerRadius_ = rhs.innerRadius_;
    outerRadius_ = rhs.outerRadius_;
}

PortTEMCoaxial::~PortTEMCoaxial() {
}

CVecR3 PortTEMCoaxial::getOrigin(const BoundTerminations& sym) const {
    return origin_;
}

bool PortTEMCoaxial::hasSameProperties(const PortTEMCoaxial& rhs) const {
    bool res = true;
    res &= PortTEM::hasSameProperties(rhs);
    res &= origin_ == rhs.origin_;
    res &= innerRadius_ == rhs.innerRadius_;
    res &= outerRadius_ == rhs.outerRadius_;
    return res;
}

const string& PortTEMCoaxial::getName() const {
    const static string res = "Coaxial_TEM_port";
    return res;
}

CVecR3 PortTEMCoaxial::getWeight(
        const CVecR3& pos,
        const BoundTerminations& sym) const {
    // Return normalized weights for electric field components.
    const Real rho = (pos - getOrigin()).norm();
    switch (getExcitationMode()) {
    case ExcitationMode::voltage:
    {
        const CVecR3 rhoHat = (pos - getOrigin()).normalize();
        return rhoHat / (rho * log(outerRadius_/innerRadius_));
    }
    case ExcitationMode::current:
    {
        const CVecR3 phiHat = (CVecR3(0,0,1) ^ pos).normalize();
        return phiHat / (2*M_PI*rho);
    }
    default:
        throw Error("Unsupported excitation mode.");
    }
}
