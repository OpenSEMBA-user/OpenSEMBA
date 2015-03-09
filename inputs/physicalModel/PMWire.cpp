/*
 * PMWire.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMWire.h"

PMWire::PMWire(
 const MatId id,
 const string name,
 const double radius,
 const double resistance,
 const double inductance) : PhysicalModel(id, name) {
	radius_ = radius;
	resistance_ = resistance;
	inductance_ = inductance;
}

PMWire::~PMWire() {
}

ClassBase* PMWire::clone() const {
   return new PMWire(*this);
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
	cout<< " --- Wire info ---" << endl;
	PhysicalModel::printInfo();
	cout << " Radius: " << radius_ << endl
	    << " Resistance: " << resistance_ << endl
		<< " Inductance: " << inductance_ << endl;
}
