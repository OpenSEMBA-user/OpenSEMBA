/*
 * PMMultiportPredefined.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMMultiportPredefined.h"

PMMultiportPredefined::PMMultiportPredefined() {
	type_ = undefined;
}

PMMultiportPredefined::PMMultiportPredefined(
 const uint idIn,
 const string nameIn,
 const PMMultiport::Type type) {
	id_ = idIn;
	name_ = nameIn;
	type_ = type;
}

PMMultiportPredefined::~PMMultiportPredefined() {
}

void
PMMultiportPredefined::printInfo() const {
	cout<< " --- Multiport Info --- " << endl
	    << " Id: " << id_ << endl
	    << " Name: " << name_ << endl
	    << " Type: " << getTypeStr() << endl;
}
