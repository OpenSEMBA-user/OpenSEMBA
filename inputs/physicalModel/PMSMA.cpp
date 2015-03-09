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

ClassBase* PMSMA::clone() const {
   return new PMSMA(*this);
}

void
PMSMA::printInfo() const {
	cout << "--- PMSMA info ---" << endl;
	cout << "Silver-Muller Absorbing" << endl;
	PMPredefined::printInfo();
}
