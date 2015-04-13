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
 const Real radius,
 const Real resistance,
 const Real inductance) : PhysicalModel(id, name) {
	radius_ = radius;
	resistance_ = resistance;
	inductance_ = inductance;
}

PMWire::~PMWire() {
}

Real
 PMWire::getRadius() const {
	return radius_;
}

Real
 PMWire::getResistance() const {
	return resistance_;
}

Real
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
