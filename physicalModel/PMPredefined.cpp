/*
 * PMPredefined.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPredefined.h"

PMPredefined::PMPredefined(const MatId id, const string name)
: PhysicalModel(id, name) {
}

PMPredefined::~PMPredefined() {
}

void
PMPredefined::printInfo() const {
	cout << "--- PMPredefined info ---" << endl;
	PhysicalModel::printInfo();
}
