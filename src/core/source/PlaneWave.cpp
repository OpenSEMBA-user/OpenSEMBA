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

#include "PlaneWave.h"

namespace SEMBA {
namespace Source {

PlaneWave::PlaneWave() {

}

PlaneWave::PlaneWave(Magnitude::Magnitude* magnitude,
                     Geometry::Element::Group<Geometry::Vol> elem,
                     Math::CVecR3 direction,
                     Math::CVecR3 polarization)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    direction_ = direction;
    polarization_ = polarization;
    if (polarization_.norm() == 0) {
        throw Error::PlaneWave::ZeroPolarization();
    }
    if (direction_.norm() == 0) {
        throw Error::PlaneWave::ZeroMagnitude();
    }
    Math::Real dotProd = direction.dot(polarization);
    if (Math::Util::notEqual(dotProd, 0.0)) {
        throw Error::PlaneWave::NotPerpendicular();
    }
}

PlaneWave::PlaneWave(const PlaneWave& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Vol>(rhs) {

    direction_ = rhs.direction_;
    polarization_ = rhs.polarization_;
}

PlaneWave::~PlaneWave() {

}

bool PlaneWave::hasSameProperties(const SEMBA::Source::Base& rhs) const {
    if(!SEMBA::Source::Base::hasSameProperties(rhs)) {
        return false;
    }
    const PlaneWave* rhsPtr = rhs.castTo<PlaneWave>();
    bool hasSameProperties = true;
    hasSameProperties &= direction_ == rhsPtr->direction_;
    hasSameProperties &= polarization_ == rhsPtr->polarization_;
    return hasSameProperties;
}

const std::string& PlaneWave::getName() const {
    const static std::string res = "PlaneWave";
    return res;
}

const Math::CVecR3& PlaneWave::getPolarization() const {
    return polarization_;
}

const Math::CVecR3& PlaneWave::getWaveDirection() const {
    return direction_;
}

Math::Real PlaneWave::getTheta() const {
    return cartesianToPolar (direction_).first;
}

Math::Real PlaneWave::getPhi() const {
    return cartesianToPolar (direction_).second;
}

Math::Real PlaneWave::getAlpha() const {
    return cartesianToPolar (polarization_).first;
}

Math::Real PlaneWave::getBeta() const {
    return cartesianToPolar (polarization_).second;
}

Math::CVecR3
PlaneWave::getElectricField(const Math::Real time) const {
    Math::CVecR3 res = polarization_ * getMagnitude()->evaluate(time);
    return res;
}

std::pair<Math::CVecR3, Math::CVecR3>
PlaneWave::getElectromagneticField(const Math::Real time) const {
    Math::CVecR3 electric = getElectricField(time);
    Math::CVecR3 magnetic = (direction_ ^ electric) *
                            Math::Constants::VACUUM_ADMITANCE;
    return std::pair<Math::CVecR3,Math::CVecR3>(electric, magnetic);
}

void PlaneWave::printInfo() const {
    std::cout<< " --- PlaneWave info --- " << std::endl;
    SEMBA::Source::Base::printInfo();
    std::cout<< " - Polarization vector: " << polarization_ << std::endl;
    std::cout<< " - Wave direction vector: " << direction_ << std::endl;
}

std::pair<Math::Real,Math::Real> PlaneWave::cartesianToPolar(
        const Math::CVecR3& v) const {
    Math::Real vx_, vy_,vz_;
    Math::Real vmxyz, vmxy_, alpha_aux, beta_aux;
    vmxyz = std::sqrt(v(Math::Constants::x)*v(Math::Constants::x)+
                      v(Math::Constants::y)*v(Math::Constants::y)+
                      v(Math::Constants::z)*v(Math::Constants::z));
    vx_ = v(Math::Constants::x)/vmxyz;
    vy_ = v(Math::Constants::y)/vmxyz;
    vz_ = v(Math::Constants::z)/vmxyz;
    vmxy_  = sqrt(vx_*vx_+vy_*vy_);
    alpha_aux = acos(vz_); //acos(Ez) [0, pi]
    if(vy_>0.0){
        beta_aux = std::abs(acos(vx_/vmxy_));
    } else if(v(Math::Constants::y)==0.0){
        beta_aux = 0.0;
    } else {
        beta_aux = -std::abs(acos(vx_/vmxy_));
    }
    std::pair<Math::Real,Math::Real> res;
    res.first = reduceRadians(alpha_aux); // alpha_aux % (2*Constants::pi) ?
    res.second = reduceRadians(beta_aux);  // beta_aux % (2*Constants::pi) ?
    return res;
}

Math::Real PlaneWave::reduceRadians(const Math::Real radianIn) const {
    Math::Real nVueltas, nVueltasComp, radianOut, Val2Pi;
    Val2Pi = (Math::Real) 2.0 * (Math::Real) acos((Math::Real) 0.0);
    nVueltas = radianIn/(Val2Pi);
    nVueltasComp = (Math::Real) floor(nVueltas);
    radianOut = radianIn - nVueltasComp*Val2Pi;
    return  radianOut;
}

} /* namespace Source */
} /* namespace SEMBA */
