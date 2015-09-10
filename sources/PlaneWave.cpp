/*
 * PlaneWave.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "../sources/PlaneWave.h"

PlaneWave::ErrorZeroPolarization::ErrorZeroPolarization()
:   Error("PlaneWave: Polarization can't be zero.") {

}

PlaneWave::ErrorZeroPolarization::~ErrorZeroPolarization() throw () {

}

PlaneWave::ErrorZeroMagnitude::ErrorZeroMagnitude()
:   Error("PlaneWave: W. direction can't be zero.") {

}

PlaneWave::ErrorZeroMagnitude::~ErrorZeroMagnitude() throw () {

}

PlaneWave::ErrorNotPerpendicular::ErrorNotPerpendicular()
:   Error("PlaneWave: W. direction is not perpendicular to polarization.") {

}

PlaneWave::ErrorNotPerpendicular::~ErrorNotPerpendicular() throw () {

}

PlaneWave::PlaneWave() {

}

PlaneWave::PlaneWave(Magnitude* magnitude,
                     GroupElements<Vol> elem,
                     CVecR3 direction,
                     CVecR3 polarization)
:   EMSourceBase(magnitude),
    GroupElements<const Vol>(elem) {

    direction_ = direction;
    polarization_ = polarization;
    if (polarization_.norm() == 0) {
        throw ErrorZeroPolarization();
    }
    if (direction_.norm() == 0) {
        throw ErrorZeroMagnitude();
    }
    //
    if ((direction ^ polarization).norm() !=
         direction.norm() * polarization.norm()) {
        throw ErrorNotPerpendicular();
    }
}

PlaneWave::PlaneWave(const PlaneWave& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Vol>(rhs) {

    direction_ = rhs.direction_;
    polarization_ = rhs.polarization_;
}

PlaneWave::~PlaneWave() {

}

bool PlaneWave::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const PlaneWave* rhsPtr = rhs.castTo<PlaneWave>();
    bool hasSameProperties = true;
    hasSameProperties &= direction_ == rhsPtr->direction_;
    hasSameProperties &= polarization_ == rhsPtr->polarization_;
    return hasSameProperties;
}

const string& PlaneWave::getName() const {
    const static string res = "PlaneWave";
    return res;
}

const CVecR3& PlaneWave::getPolarization() const {
    return polarization_;
}

const CVecR3& PlaneWave::getWaveDirection() const {
    return direction_;
}

Real PlaneWave::getTheta() const {
    return cartesianToPolar (direction_).first;
}

Real PlaneWave::getPhi() const {
    return cartesianToPolar (direction_).second;
}

Real PlaneWave::getAlpha() const {
    return cartesianToPolar (polarization_).first;
}

Real PlaneWave::getBeta() const {
    return cartesianToPolar (polarization_).second;
}

CVecR3
PlaneWave::getElectricField(const Real time) const {
    CVecR3 res = polarization_ * getMagnitude()->evaluate(time);
    return res;
}

pair<CVecR3, CVecR3>
PlaneWave::getElectromagneticField(const Real time) const {
    CVecR3 electric = getElectricField(time);
    CVecR3 magnetic = (direction_ ^ electric) * (Real) VACUUM_ADMITANCE;
    return pair<CVecR3,CVecR3>(electric, magnetic);
}

void PlaneWave::printInfo() const {
	cout<< " --- PlaneWave info --- " << endl;
	EMSourceBase::printInfo();
	cout<< " - Polarization vector: " << polarization_ << endl;
	cout<< " - Wave direction vector: " << direction_ << endl;
}

pair<Real,Real> PlaneWave::cartesianToPolar(const CVecR3& v) const {
    Real vx_, vy_,vz_;
    Real vmxyz, vmxy_, alpha_aux, beta_aux;
    vmxyz = sqrt(v(x)*v(x)+v(y)*v(y)+v(z)*v(z));
    vx_ = v(x)/vmxyz;
    vy_ = v(y)/vmxyz;
    vz_= v(z)/vmxyz;
    vmxy_  = sqrt(vx_*vx_+vy_*vy_);
    alpha_aux = acos(vz_); //acos(Ez) [0, pi]
    if(vy_>0.0){
        beta_aux = abs(acos(vx_/vmxy_));
    } else if(v(y)==0.0){
        beta_aux = 0.0;
    } else {
        beta_aux = -abs(acos(vx_/vmxy_));
    }
    pair<Real,Real> res;
    res.first = reduceRadians(alpha_aux); // alpha_aux % (2*M_PI) ?
    res.second = reduceRadians(beta_aux);  // beta_aux % (2*M_PI) ?
    return res;
}

Real PlaneWave::reduceRadians(const Real radianIn) const {
    Real nVueltas, nVueltasComp, radianOut, Val2Pi;
    Val2Pi = (Real) 2.0 * (Real) acos((Real) 0.0);
    nVueltas = radianIn/(Val2Pi);
    nVueltasComp = (Real) floor(nVueltas);
    radianOut = radianIn - nVueltasComp*Val2Pi;
    return  radianOut;
}
