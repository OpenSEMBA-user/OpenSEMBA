/*
 * PMMultiportPredefined.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMMultiportPredefined.h"

PMMultiportPredefined::PMMultiportPredefined(
 const MatId id,
 const string name,
 const PMMultiport::Type type) : PMMultiport(id, name) {
	type_ = type;
}

PMMultiportPredefined::~PMMultiportPredefined() {
}

void
PMMultiportPredefined::printInfo() const {
	cout<< " --- Multiport Info --- " << endl;
	PMMultiport::printInfo();
	cout<< " Type: " << getTypeStr() << endl;
}
