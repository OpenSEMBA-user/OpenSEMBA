/*
 * MeshStructured.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#include "MeshUnstructured.h"
#include "MeshStructured.h"

MeshStructured::MeshStructured(const Grid3& grid)
:   Grid3(grid) {

}

MeshStructured::MeshStructured(const Grid3& grid,
                               const CoordinateGroup<const CoordI3>& cG,
                               const ElementsGroup<const ElemI>& elem,
                               const LayerGroup<const Layer>& layers)
:   Grid3(grid),
    CoordinateGroup<CoordI3>(cG.newGroup()),
    ElementsGroup<ElemI>(elem.newGroup()),
    LayerGroup<Layer>(layers.newGroup()) {

    ElementsGroup<ElemI>::reassignPointers(*this);
}

MeshStructured::MeshStructured(const MeshStructured& rhs)
:   Grid3(rhs),
    CoordinateGroup<CoordI3>(rhs.coords().newGroup()),
    ElementsGroup<ElemI>(rhs.elems().newGroup()),
    LayerGroup<Layer>(rhs.layers().newGroup()) {

    ElementsGroup<ElemI>::reassignPointers(*this);
}

MeshStructured::~MeshStructured() {

}

MeshStructured& MeshStructured::operator=(const MeshStructured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    Grid3::operator=(rhs);
    CoordinateGroup<CoordI3>::operator=(rhs.coords().newGroup());
    ElementsGroup<ElemI>::operator=(rhs.elems().newGroup());
    LayerGroup<Layer>::operator=(rhs.layers().newGroup());

    ElementsGroup<ElemI>::reassignPointers(*this);

    return *this;
}

void MeshStructured::applyScalingFactor(const Real factor) {
    Grid3::applyScalingFactor(factor);
}

vector<BoxR3>
MeshStructured::getRectilinearHexesInsideRegion(
        const ElementsGroup<ElemR>& region) const {

    vector<CVecR3> center = getCenterOfCellsInside(region.getBound());
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

MeshUnstructured* MeshStructured::getMeshUnstructured() const {
    MeshUnstructured* res = new MeshUnstructured;

    vector<CoordR3*> newCoords;
    newCoords.reserve(coords().size());
    for (UInt i = 0; i < coords().size(); i++) {
        CoordR3* newCoord = coords()(i)->toUnstructured(*this);
        if (newCoord != NULL) {
            newCoords.push_back(newCoord);
        }
    }
    res->coords().add(newCoords);

    vector<ElemR*> newElems;
    newElems.reserve(elems().size());
    for (UInt i = 0; i < elems().size(); i++) {
        ElemR* newElem = elems()(i)->toUnstructured(*res, *this);
        if (newElem != NULL) {
            newElems.push_back(newElem);
        }
    }
    res->elems().add(newElems);
    res->layers() = layers().newGroup();
    return res;
}

void MeshStructured::printInfo() const {
    cout << " --- Mesh structured info --- " << endl;
    Grid3::printInfo();
    CoordinateGroup<CoordI3>::printInfo();
    ElementsGroup<ElemI>::printInfo();
    LayerGroup<>::printInfo();
}


vector<BoxR3> MeshStructured::discretizeWithinBoundary(
        const MatId matId,
        const LayerId layId) const {
    ElementsGroup<const SurfR> surfs =
        elems().getGroupWith(matId, layId).getGroupOf<SurfR>();
    return discretizeWithinBoundary(this, surfs);
}

vector<BoxR3> MeshStructured::discretizeWithinBoundary(
const Grid3* grid, const ElementsGroup<const SurfR>& faces) const {
#warning "Not implemented"
}

vector<BoxR3> MeshStructured::discretizeWithinBoundary(
        const ElementsGroup<const SurfI>& surf) const {
//    checkAllFacesAreRectangular();
    // Gets pairs of quads that define the volume of the space within them.
    const vector<pair<const SurfI*, const SurfI*> > pairs =
            getPairsDefiningVolumeWithin(surf);
    // Gets positions in z-axis.
    vector<BoxR3> box(pairs.size());
    vector<vector<Real> > zPos(pairs.size());
    for (UInt p = 0; p < pairs.size(); p++) {
        CVecR3 min = getPos(*pairs[p].first->getMinV());
        CVecR3 max = getPos(*pairs[p].second->getMaxV());
        box[p] = BoxR3(min,max);
        if (min(2) > max(2)) {
            zPos[p] = getPosInRange(z, max(2), min(2));
        } else {
            zPos[p] = getPosInRange(z, min(2), max(2));
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

vector<pair<const SurfI*, const SurfI*> >
MeshStructured::getPairsDefiningVolumeWithin(
        const ElementsGroup<const SurfI>& origBound) const {
    vector<pair<const SurfI*, const SurfI*> > res;
    const UInt nOrigBound = origBound.size();
    // Checks that bound.size is an even number.
    if (nOrigBound % 2 != 0) {
        cerr << endl << "ERROR @ Mesh:"
                << "Bound size must be an even number to be closed." << endl;
        return res;
    }
    // Remove Surfaces not lying in a xy plane.
    vector<const SurfI*> bound;
    bound.reserve(nOrigBound);
    for (UInt b = 0; b < origBound.size(); b++) {
        if (origBound(b)->isContainedInPlane(xy)) {
            bound.push_back(origBound(b));
        }
    }
    // Sort remaining quad Ids using coordinates as a lexicographical order.
    // Pairs quadrilaterals that are aligned in the same Z natural axe.
    const UInt nBound = bound.size();
    assert(nBound % 2 == 0);
    res.reserve(nBound / 2);
    // Sorts.
    DynMatrix<Int> quads(nBound, 4);
    for (UInt b = 0; b < nBound; b++) {
        CVecI3 minPos = bound[b]->getMinV()->pos();
        // Stores boundary at quad list.
        quads(b, 0) = (Real) bound[b]->getId();
        for (UInt i = 0; i < 3; i++) {
            quads(b, i+1) = minPos(i);
        }
    }
    quads.sortRows_omp(1,3);
    // Performs pairing.
    pair<const SurfI*, const SurfI*> aux;
    for (UInt b = 0; b < nBound; b++) {
        const ElementId id(quads(b,0));
        if (b % 2 == 0) {
            aux.first = elems().get(id)->castTo<SurfI>();
        } else {
            aux.second = elems().get(id)->castTo<SurfI>();
            res.push_back(aux);
        }
    }
    return res;
}
