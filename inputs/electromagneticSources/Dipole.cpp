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
 vector<unsigned int> elem_,
 double length_,
 CartesianVector<double,3> orientation_,
 CartesianVector<double,3> position_,
 bool sinModulation_, double sinAmplitude_, double frequency_,
 bool gaussModulation_, double gaussAmplitude_, double spread_,
 double delay_) {
	elem = elem_;
	length = length_;
	orientation = orientation_;
	position = position_;
	sinModulation = sinModulation_;
	sinAmplitude = sinAmplitude_;
	frequency = frequency_;
	gaussModulation = gaussModulation_;
	gaussAmplitude = gaussAmplitude_;
	spread = spread_;
	delay = delay_;
	// Builds other parameters used for computations.
	const double pi2 = abs(atan((double)1.0)*(double)4.0) * (double) 2.0;
	w = frequency * pi2;
	spreadSqrt2xC0 =
	 spread * sqrt((double)2.0) * SPEED_OF_LIGHT;
	invSpreadSq = double(1.0) / (spread * spread);
}

Dipole::~Dipole() {

}

Dipole&
Dipole::operator=(const Dipole& rhs) {
	if (this == &rhs) {
		return *this;
	}
	elem = rhs.elem;
	length = rhs.length;
	orientation = rhs.orientation;
	position = rhs.position;
	sinModulation = rhs.sinModulation;
	sinAmplitude = rhs.sinAmplitude;
	frequency = rhs.frequency;
	w = rhs.w;
	gaussModulation = rhs.gaussModulation;
	gaussAmplitude = rhs.gaussAmplitude;
	spread = rhs.spread;
	delay = rhs.delay;
	spreadSqrt2xC0 = rhs.spreadSqrt2xC0;
	invSpreadSq = rhs.invSpreadSq;
	return *this;
}

void
Dipole::printInfo() const {
	cout << " ---- Dipole information ---- " << endl;
	cout << " - Assigned on " << elem.size() << " elements." << endl;
	cout << " - Length: " << length << endl;
	cout << " - Orientation vector:" << endl;
    orientation.printInfo();
    cout << endl;
    cout << " - Position vector:" << endl;
    position.printInfo();
    cout << endl;
	if (sinModulation) {
		cout << " - Dipole exc. is modulated with a sinusoid." << endl;
		cout << "   - Sinusoid amplitude: " << sinAmplitude    << endl;
		cout << "   - Sinusoid frequency: "	<< frequency       << endl;
	}
	if (gaussModulation) {
		cout << " - Dipole exc. is modulated with a gaussian." << endl;
		cout << "	- Gaussian amplitude: "	<< gaussAmplitude  << endl;
		cout << "   - Gaussian spread:    "	<< spread		   << endl;
		cout << "   - Gaussian delay:	  "	<< delay		   << endl;
	}
}
