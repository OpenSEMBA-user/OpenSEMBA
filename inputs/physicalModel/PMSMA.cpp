/*
 * PMSMA.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMSMA.h"

PMSMA::~PMSMA() {

}

PMSMA::PMSMA(const uint id, const string name)
: PMPredefined(id, name) {

}

bool
PMSMA::isSMA() const {
	return true;
}

Condition::Type PMSMA::getConditionType() const {
   return Condition::sma;
}

void
PMSMA::printInfo() const {
	cout << "--- PMSMA info ---" << endl;
	PMPredefined::printInfo();
	cout << "Silver-Muller Absorbing" << endl;
}
