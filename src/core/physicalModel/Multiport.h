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
 * PMMultiport.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMMULTIPORT_H_
#define PMMULTIPORT_H_

#include "PhysicalModel.h"

class PMMultiport: public PhysicalModel {
public:
	typedef enum {
		// Predefined
		shortCircuit = 1,
		openCircuit = 2,
		matched = 3,
		// RLC types.
		sRLC = 11,    // R+L+C.
		sRLpC = 12,   // (R + L)//C.
		sRCpL = 13,   // (R + C)//L.
		sRpLC = 14,   // R + (L//C).
		sLCpR = 15,   // (L + C)//R.
		sLpRC = 16,   // L + (R//C).
		sCpLR = 17,   // C + (L//R).
		pRLC = 18,     // R//L//C.
		undefined = 0
 	} Type;
 	PMMultiport(const MatId id, const string name);
	virtual ~PMMultiport();
	virtual Type getType() const;
protected:
	Type type_;
	string getTypeStr() const;
};

#endif /* PMMULTIPORT_H_ */
