/*
 * PMWire.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMWire.h"

PMWire::PMWire() {
	radius_ = 0.0;
	resistance_ = 0.0;
	inductance_ = 0.0;
}

PMWire::PMWire(
 const uint idIN,
 const string nameIN,
 const double radius,
 const double resistance,
 const double inductance) {
	id_ = idIN;
	name_ = nameIN;
	radius_ = radius;
	resistance_ = resistance;
	inductance_ = inductance;
}

PMWire::~PMWire() {
}

double
 PMWire::getRadius() const {
	return radius_;
}

double
 PMWire::getResistance() const {
	return resistance_;
}

double
 PMWire::getInductance() const {
	return inductance_;
}

void
PMWire::printInfo() const {
	cout<< " --- Wire info ---" << endl
	    << " Id: " << id_ << endl
	    << " Name: " << name_ << endl
	    << " Radius: " << radius_ << endl
	    << " Resistance: " << resistance_ << endl
		<< " Inductance: " << inductance_ << endl;
}
