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
    Grid3::applyScalingFactor(factor);
}

vector<BoxR3>
MeshStructured::getRectilinearHexesInsideRegion(
        const ElementsGroup<VolR>& region) const {

    vector<CVecR3> center = getCenterOfNaturalCellsInside(region.getBound());
    vector<BoxR3> res;
    res.reserve(center.size());
    for (UInt i = 0; i < center.size(); i++) {
        for (UInt j = 0; j < region.size(); j++) {
            if (region(j)->isInnerPoint(center[i])) {
                res.push_back(getBoundingBoxContaining(center[i]));
                break;
            }
        }
    }
    return res;
}

void MeshStructured::printInfo() const {
    cout << " --- Mesh structured info --- " << endl;
    Grid3::printInfo();
}
