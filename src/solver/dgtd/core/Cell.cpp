/*
 * Cell.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: luis
 */

#ifndef CELL_H_
#include "Cell.h"
#endif

Cell::Cell() {
	material = NULL;
}

Cell::~Cell() {
}

vector<UInt> Cell::getNeighbourNodes(const UInt f, const MeshVolume& mesh) {
    vector<UInt> res(getNfp());
    CVecR3 diff, posM, posP;
    const ElementId id = getId();
    // Stores contiguous element (e2) number and orientation.
    const Tet* neigh = map.getNeighbour(id, f);
    const CellTet<ORDER_N>* c2 = getPtrToCell(neigh);
    const UInt f2 = map.getVolToF(id, f);
    // Runs over each node in local element.
    for (UInt i = 0; i < cell[e]->getNfp(); i++) {
        // Initializes mapP and vmapP to default values.
        cell[e]->vmapP[f][i] = cell[e]->getSideNode(f, i);
        // Creates the position vector of local element.
        posM = cell[e]->getSideNodePos(f, i);
        // Checks posM against all nodes in e2 face.
        for (UInt j = 0; j < c2->nfp; j++) {
            posP = c2->getSideNodePos(f2, j);
            diff = posM - posP;
            // Stores value if the share position.
            if (MathUtils::equal(diff.norm(), (Real) 0.0)) {
                res[i] = getSideNode(f2, j);
            }
        }
    }
}
