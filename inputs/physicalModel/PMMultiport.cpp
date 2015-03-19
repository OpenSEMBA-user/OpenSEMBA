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

