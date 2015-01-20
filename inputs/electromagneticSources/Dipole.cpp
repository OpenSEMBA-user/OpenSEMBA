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
 vector<unsigned int> elem,
 double length,
 CVecD3 orientation,
 CVecD3 position,
 const MagnitudeGaussian* magnitude) : EMSource(elem, magnitude) {
	length_ = length;
	orientation_ = orientation;
	position_ = position;
	gaussDelay_ = magnitude->getDelay();
	spreadSqrt2_ = magnitude->getSpread() * M_SQRT2;
}

Dipole::~Dipole() {

}

Dipole&
Dipole::operator=(const Dipole& rhs) {
	if (this == &rhs) {
		return *this;
	}
	EMSource::operator=(rhs);
	length_ = rhs.length_;
	orientation_ = rhs.orientation_;
	position_ = rhs.position_;
	gaussDelay_ = rhs.gaussDelay_;
	spreadSqrt2_ = rhs.spreadSqrt2_;
	return *this;
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
