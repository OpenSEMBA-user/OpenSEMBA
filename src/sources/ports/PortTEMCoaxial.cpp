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
        EMSourceBase(magnitude),
        GroupElements<const Surf>(elem),
        PortTEM(magnitude, elem, excMode) {
    origin_ = origin;
    innerRadius_ = innerRadius;
    outerRadius_ = outerRadius;
}

PortTEMCoaxial::PortTEMCoaxial(
        const PortTEMCoaxial& rhs) :
                        EMSourceBase(rhs),
                        GroupElements<const Surf>(rhs),
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
        return phiHat / (2.0 * Constants::pi * rho);
    }
    default:
        throw Error("Unsupported excitation mode.");
    }
}

void PortTEMCoaxial::printInfo() const {
    cout << " --- Coaxial TEM Port --- " << endl;
    cout << "Origin: " << origin_ << endl;
    cout << "Inner/Outer radii: "
            << innerRadius_ << "/" << outerRadius_ << endl;
    PortTEM::printInfo();
}

void PortTEMCoaxial::set(
        const GroupElements<const Elem>& elemGroup) {
    // Reescales internal dimensions.
    BoxR3 box = elemGroup.getBound();
    const CVecR3 diagonal = box.getMax()-box.getMin();
    if (!diagonal.isContainedInPlane(CartesianPlane::xy)) {
        throw Error("Port is not contained in a XY plane");
    }
    const Real width = box.getMax()(x) - box.getMin()(x);
    const Real height = box.getMax()(y) - box.getMin()(y);
    const Real averageNewRadius = (width + height)/4;
    innerRadius_ *= averageNewRadius;
    outerRadius_ *= averageNewRadius;
    const CVecR3 averageNewOrigin = (box.getMax() + box.getMin()) / 2;
    origin_ = averageNewOrigin;
    //
    EMSource<Surf>::set(elemGroup);
}
