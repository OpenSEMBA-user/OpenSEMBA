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


vector<BoxR3> MeshStructured::discretizeWithinBoundary(
        const MatId matId,
        const LayerId layId) const {
    ElementsGroup<SurfR> surfs = elems().get(matId, layId).getGroupOf<SurfR>();
    return discretizeWithinBoundary(grid, surfs);
}

vector<BoxR3> MeshStructured::discretizeWithinBoundary(
        const ElementsGroup<SurfR>& surf) const {
    checkAllFacesAreRectangular();
    // Gets pairs of quads that define the volume of the space within them.
    const vector<pair<const SurfR*, const SurfR*> > pairs =
            getPairsDefiningVolumeWithin(grid, surf);
    // Gets positions in z-axis.
    vector<BoxR3> box(pairs.size());
    vector<vector<Real> > zPos(pairs.size());
    for (UInt p = 0; p < pairs.size(); p++) {
        CVecR3 min = pairs[p].first->getMinV()->pos();
        CVecR3 max = pairs[p].second->getMaxV()->pos();
        box[p] = BoxR3(min,max);
        if (min(2) > max(2)) {
            zPos[p] = grid->getPosInRange(z, max(2), min(2));
        } else {
            zPos[p] = grid->getPosInRange(z, min(2), max(2));
        }
        assert(zPos[p].size() > 0);
    }
    // Fills the space within pairs with hexahedrons aligned with the grid.
    UInt nHex = 0;
    for (UInt p = 0; p < zPos.size(); p++) {
        nHex += zPos[p].size() - 1;
    }
    vector<BoxR3> res;
    res.reserve(nHex);
    for (UInt p = 0; p < pairs.size(); p++) {
        for (UInt i = 1; i < zPos[p].size(); i++) {
            CVecR3 min(box[p].getMin()(0), box[p].getMin()(1), zPos[p][i-1]);
            CVecR3 max(box[p].getMax()(0), box[p].getMax()(1), zPos[p][i]);
            res.push_back(BoxR3(min, max));
        }
    }
    return res;
}
