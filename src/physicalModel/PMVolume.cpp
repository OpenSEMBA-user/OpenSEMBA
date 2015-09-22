/*
 * PMVolume.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolume.h"

PMVolume::PMVolume(const MatId id, const string name)
: PhysicalModel(id, name) {
}

PMVolume::~PMVolume() {

}

void PMVolume::printInfo() const {
	cout << "--- PMVolume info ---" << endl;
	PhysicalModel::printInfo();
}

