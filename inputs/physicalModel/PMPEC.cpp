/*
 * PMPEC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPEC.h"


// =============== PMPEC ======================================================
PMPEC::PMPEC(const MatId id, const string name)
: PMPredefined(id, name) {

}

PMPEC::~PMPEC() {

}

ClassBase* PMPEC::clone() const {
    return new PMPEC(*this);
}

void
PMPEC::printInfo() const {
	cout << "--- PMPEC info ---" << endl;
	PMPredefined::printInfo();
	cout << "Perfect Electric Conductor" << endl;
}
