/*
 * MeshStructured.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#include "MeshStructured.h"

MeshStructured::~MeshStructured() {
    // TODO Auto-generated destructor stub
}

MeshStructured::MeshStructured() {
    // TODO Auto-generated constructor stub

}

vector<BoxR3>
MeshStructured::discretizeWithinBoundary(
        const UInt matId, const UInt layId) const {
#warning "Not implemented"
}

void
MeshStructured::applyScalingFactor(const Real factor) {
    Mesh::applyScalingFactor(factor);
    Grid3::applyScalingFactor(factor);
}

void MeshStructured::printInfo() const {
    cout << " --- Mesh structured info --- " << endl;
    Mesh::printInfo();
    Grid3::printInfo();
}
