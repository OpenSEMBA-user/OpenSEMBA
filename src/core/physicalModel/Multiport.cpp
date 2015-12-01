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
 * PMMultiport.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMMultiport.h"

PMMultiport::PMMultiport(const MatId id, const string name)
: PhysicalModel(id, name) {
    type_ = undefined;
}

PMMultiport::~PMMultiport() {
	// TODO Auto-generated destructor stub
}

PMMultiport::Type
PMMultiport::getType() const {
	return type_;
}

string
PMMultiport::getTypeStr() const {
	string res;
	switch (type_) {
	case shortCircuit:
		res = "Short circuit";
		break;
	case openCircuit:
		res = "Open circuit";
		break;
	case matched:
		res = "Matched";
		break;
	case sRLC:
		res = "Series RLC";
		break;
	case sRLpC:
		res = "Series RL Parallel C";
		break;
	case sRCpL:
		res = "Series RC Parallel L";
		break;
	case sRpLC:
		res = "Series R Parallel C";
		break;
	case sLCpR:
		res = "Series LC Parallel R";
		break;
	case sLpRC:
		res = "Series L Parallel RC";
		break;
	case sCpLR:
		res = "Series C Parallel LR";
		break;
	case pRLC:
		res = "Parallel RLC";
		break;
	default:
		res = "Undefined";
		break;
	}
	return res;
}

