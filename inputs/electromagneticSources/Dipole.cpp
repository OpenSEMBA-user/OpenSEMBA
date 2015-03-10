/*
 * Dipole.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef DIPOLE_H_
	#include "Dipole.h"
#endif



Dipole::Dipole(
 vector<ElementId> elem,
 double length,
 CVecD3 orientation,
 CVecD3 position,
 const MagnitudeGaussian* magnitude) : EMSource(elem, magnitude) {
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
