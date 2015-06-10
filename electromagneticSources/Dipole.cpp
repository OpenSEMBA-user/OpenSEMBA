/*
 * Dipole.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Dipole.h"

Dipole::Dipole(const MagnitudeGaussian* magnitude,
               const GroupElements<Vol>& elem,
               Real length,
               CVecR3 orientation,
               CVecR3 position)
:   EMSourceBase(magnitude),
    GroupElements<const Vol>(elem) {

    length_ = length;
    orientation_ = orientation;
    position_ = position;
    gaussDelay_ = magnitude->getDelay();
    spreadSqrt2_ = magnitude->getSpread() * sqrt(2.0);
}

Dipole::Dipole(const Dipole& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Vol>(rhs) {

    length_ = rhs.length_;
    orientation_ = rhs.orientation_;
    position_ = rhs.position_;
    gaussDelay_ = rhs.gaussDelay_;
    spreadSqrt2_ = rhs.spreadSqrt2_;
}

Dipole::~Dipole() {

}

bool Dipole::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const Dipole* rhsPtr = rhs.castTo<Dipole>();
    bool hasSameProperties = true;
    hasSameProperties &= length_ == rhsPtr->length_;
    hasSameProperties &= orientation_ == rhsPtr->orientation_;
    hasSameProperties &= position_ == rhsPtr->position_;
    hasSameProperties &= gaussDelay_ == rhsPtr->gaussDelay_;
    hasSameProperties &= spreadSqrt2_ == rhsPtr->spreadSqrt2_;
    return hasSameProperties;
}

const string& Dipole::getName() const {
    const static string res = "Dipole";
    return res;
}

void Dipole::printInfo() const {
    cout << " ---- Dipole information ---- " << endl;
    EMSourceBase::printInfo();
    cout << " - Length: " << length_ << endl;
    cout << " - Orientation vector:";;
    orientation_.printInfo();
    cout << endl;
    cout << " - Position vector:";
    position_.printInfo();
    cout << endl;
}
