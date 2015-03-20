/*
 * Dipole.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Dipole.h"

Dipole::Dipole(const MagnitudeGaussian* magnitude,
               const ElementsGroup<Volume<> >& elem,
               Real length,
               CVecR3 orientation,
               CVecR3 position)
:   EMSource<>(magnitude),
    ElementsGroup<Volume<> >(elem) {

    length_ = length;
    orientation_ = orientation;
    position_ = position;
    gaussDelay_ = magnitude->getDelay();
    spreadSqrt2_ = magnitude->getSpread() * sqrt(2.0);
}

Dipole::Dipole(const Dipole& rhs)
:   EMSource<>(rhs),
    ElementsGroup<Volume<> >(rhs) {

    length_ = rhs.length_;
    orientation_ = rhs.orientation_;
    position_ = rhs.position_;
    gaussDelay_ = rhs.gaussDelay_;
    spreadSqrt2_ = rhs.spreadSqrt2_;
}

Dipole::~Dipole() {

}

ClassBase* Dipole::clone() const {
    return new Dipole(*this);
}

void Dipole::printInfo() const {
    cout << " ---- Dipole information ---- " << endl;
    EMSource<>::printInfo();
    cout << " - Length: " << length_ << endl;
    cout << " - Orientation vector:";;
    orientation_.printInfo();
    cout << endl;
    cout << " - Position vector:";
    position_.printInfo();
    cout << endl;
}
