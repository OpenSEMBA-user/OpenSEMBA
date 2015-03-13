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
MeshStructured::getRectilinearHexesInsideRegion(
        const ElementsGroup<ElemR>& region) const {
    vector<CVecR3> center = getCenterOfNaturalCellsInside(region.getBound());
    vector<BoxR3> res;
    res.reserve(center.size());
    for (UInt i = 0; i < center.size(); i++) {
        for (UInt j = 0; j < region.size(); j++) {
            if (region(j)->castTo<VolR>()->isInnerPoint(center[i])) {
                res.push_back(getBoundingBoxContaining(center[i]));
                break;
            }
        }
    }
    return res;
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
