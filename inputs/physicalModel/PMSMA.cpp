/*
 * PMSMA.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMSMA.h"

PMSMA::~PMSMA() {

}

PMSMA::PMSMA(const MatId id, const string name)
: PMPredefined(id, name) {

}

void
PMSMA::printInfo() const {
	cout << "--- PMSMA info ---" << endl;
	cout << "Silver-Muller Absorbing" << endl;
	PMPredefined::printInfo();
}
