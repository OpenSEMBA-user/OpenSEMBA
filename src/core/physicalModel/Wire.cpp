// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
