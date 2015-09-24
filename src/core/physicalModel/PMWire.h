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
 * PMWire.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMWIRE_H_
#define PMWIRE_H_

#include "PhysicalModel.h"

class PMWire: public PhysicalModel {
public:
	PMWire(
	 const MatId id,
	 const string name,
	 const Real radius,
	 const Real resistance,
	 const Real inductance);
	virtual ~PMWire();

    DEFINE_CLONE(PMWire);

	virtual Real	 getRadius() const;
	virtual Real	 getResistance() const;
	virtual Real	 getInductance() const;
	virtual void printInfo() const;
private:
	Real radius_;
	Real resistance_; // Resistance per meter.
	Real inductance_; // Inductance per meter.
};

#endif /* PMWIRE_H_ */
