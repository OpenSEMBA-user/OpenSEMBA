/*
 * PMPMC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPMC.h"

PMPMC::~PMPMC() {

}

PMPMC::PMPMC(const unsigned int id, const string name)
: PMPredefined(id, name){
}

ClassBase* PMPMC::clone() const {
   return new PMPMC(*this);
}

void
PMPMC::printInfo() const {
	cout << "--- PMPMC info ---" << endl;
	PMPredefined::printInfo();
	cout << "Perfect Magnetic Conductor" << endl;
}
