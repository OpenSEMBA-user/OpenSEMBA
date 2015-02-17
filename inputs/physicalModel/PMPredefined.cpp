/*
 * PMPredefined.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPredefined.h"

PMPredefined&
PMPredefined::operator=(const PMPredefined &param) {
	if (this == &param) {
		return *this;
	}
	// Copies values to object.
	id_ = param.id_;
	name_ = param.name_;
	return *this;
}

void
PMPredefined::printInfo() const {
	cout << "--- PMPredefined info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Type: ";
	cout << "UNKNOWN" << endl;
	cerr << "ERROR@PMPredefined::printInfo()" << endl;
	cerr << "Unable to determine type." << endl;
	exit(PHYSICALMODEL_ERROR);
}
