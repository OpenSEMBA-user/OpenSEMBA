/*
 * Dipole.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "Dipole.h"

Dipole::Dipole(
 const ElementsGroup<Volume<> >& elem,
 Real length,
 CVecR3 orientation,
 CVecR3 position,
 const MagnitudeGaussian* magnitude): EMSource(magnitude), ElementsGroup<Volume<> >(elem) {
	length_ = length;
	orientation_ = orientation;
	position_ = position;
	gaussDelay_ = magnitude->getDelay();
	spreadSqrt2_ = magnitude->getSpread() * sqrt(2.0);
}

Dipole::~Dipole() {

}

ClassBase* Dipole::clone() const {
    return new Dipole(*this);
}

void
Dipole::printInfo() const {
	cout << " ---- Dipole information ---- " << endl;
	EMSource::printInfo();
	cout << " - Length: " << length_ << endl;
	cout << " - Orientation vector:" << endl;
    orientation_.printInfo();
    cout << endl;
    cout << " - Position vector:" << endl;
    position_.printInfo();
    cout << endl;
}
