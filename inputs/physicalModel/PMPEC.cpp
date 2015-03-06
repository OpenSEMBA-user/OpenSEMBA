/*
 * PMPEC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPEC.h"


// =============== PMPEC ======================================================
PMPEC::PMPEC(const unsigned int id, const string name)
: PMPredefined(id, name) {

}

PMPEC::~PMPEC() {

}

ClassBase* PMPEC::clone() const {
    return new PMPEC(*this);
}

ClassBase* PMPEC::clone(const uint id) const {
    return new PMPEC(id, getName());
}

void
PMPEC::printInfo() const {
	cout << "--- PMPEC info ---" << endl;
	PMPredefined::printInfo();
	cout << "Perfect Electric Conductor" << endl;
}
