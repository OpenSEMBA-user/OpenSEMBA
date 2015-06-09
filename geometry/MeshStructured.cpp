/*
 * MeshStructured.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#include "MeshUnstructured.h"
#include "MeshStructured.h"

MeshStructured::ErrorInvalidBoundary::ErrorInvalidBoundary()
:   Error("Invalid boundary to discretize") {

}

MeshStructured::ErrorInvalidBoundary::~ErrorInvalidBoundary() throw () {

}

MeshStructured::MeshStructured(const Grid3& grid)
:   Grid3(grid) {

}

MeshStructured::MeshStructured(const Grid3& grid,
                               const GroupCoordinates<const CoordI3>& cG,
                               const GroupElements<const ElemI>& elem,
                               const GroupLayers<const Layer>& layers)
:   Grid3(grid),
    GroupCoordinates<CoordI3>(cG.newGroup()),
    GroupElements<ElemI>(elem.newGroup()),
    GroupLayers<Layer>(layers.newGroup()) {

    GroupElements<ElemI>::reassignPointers(*this);
}

MeshStructured::MeshStructured(const MeshStructured& rhs)
:   Grid3(rhs),
    GroupCoordinates<CoordI3>(rhs.coords().newGroup()),
    GroupElements<ElemI>(rhs.elems().newGroup()),
    GroupLayers<Layer>(rhs.layers().newGroup()) {

    GroupElements<ElemI>::reassignPointers(*this);
}

MeshStructured::~MeshStructured() {

}

MeshStructured& MeshStructured::operator=(const MeshStructured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    Grid3::operator=(rhs);
    GroupCoordinates<CoordI3>::operator=(rhs.coords().newGroup());
    GroupElements<ElemI>::operator=(rhs.elems().newGroup());
    GroupLayers<Layer>::operator=(rhs.layers().newGroup());

    GroupElements<ElemI>::reassignPointers(*this);

    return *this;
}

void MeshStructured::applyScalingFactor(const Real factor) {
    Grid3::applyScalingFactor(factor);
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

MeshStructured* MeshStructured::getConnectivityMesh() const {
    MeshStructured* res = new MeshStructured(grid());
    res->coords() = coords().newGroup();
    GroupElements<const ElemI> elems = this->elems();
    elems.remove(MatId(0));
    GraphVertices<ElemI, CoordI3> graphLayer;
    graphLayer.init(elems, coords());
    vector<vector<const ElemI*>> comps = graphLayer.getConnectedComponents();
    for (UInt c = 0; c < comps.size(); c++) {
        stringstream layerName;
        layerName << "Component " << c+1;
        Layer* newLayer = res->layers().add(new Layer(layerName.str()), true);
        vector<ElemI*> newElemsLayer;
        newElemsLayer.resize(comps[c].size());
        for (UInt e = 0; e < comps[c].size(); e++) {
            newElemsLayer[e] = comps[c][e]->cloneTo<ElemI>();
            newElemsLayer[e]->setLayerId(newLayer->getId());
        }
        res->elems().add(newElemsLayer);
    }
    res->reassignPointers(*res);
    return res;
}

void MeshStructured::printInfo() const {
    cout << " --- Mesh structured info --- " << endl;
    Grid3::printInfo();
    GroupCoordinates<CoordI3>::printInfo();
    GroupElements<ElemI>::printInfo();
    GroupLayers<>::printInfo();
}

void MeshStructured::convertToHex(GroupElements<const SurfI> surfs) {
    vector<HexI8*> hexes = discretizeWithinBoundary(surfs);
    this->GroupElements<ElemI>::remove(surfs.getIds());
    elems().add(hexes, true);
}

void MeshStructured::addAsHex(GroupElements<const VolR> region) {
    vector<CVecR3> center = getCenterOfCellsInside(region.getBound());
    vector<HexI8*> hexes;
    hexes.reserve(center.size());
    for (UInt i = 0; i < center.size(); i++) {
        for (UInt j = 0; j < region.size(); j++) {
            if (region(j)->isInnerPoint(center[i])) {
                ElementId id = region(j)->getId();
                BoxI3 box = getBoxIContaining(center[i]);
                LayerId layId = region(j)->getLayerId();
                MatId matId = region(j)->getMatId();
                hexes.push_back(new HexI8(coords(), id, box, layId, matId));
                break;
            }
        }
    }
    elems().add(hexes);
}

vector<HexI8*> MeshStructured::discretizeWithinBoundary(
        const GroupElements<const SurfI>& surf) {
    const vector<pair<const SurfI*, const SurfI*>> pair =
            getPairsDefiningVolumeWithin(surf);
    vector<HexI8*> res;
    res.reserve(pair.size());
    for (UInt i = 0; i < pair.size(); i++) {
        ElementId id(0);
        BoxI3 box(*pair[i].first->getMinV(), *pair[i].second->getMaxV());
        LayerId layId = pair[i].first->getLayerId();
        if (pair[i].first->getLayerId() != pair[i].second->getLayerId()) {
            throw ErrorInvalidBoundary();
        }
        MatId matId = pair[i].first->getMatId();
        if (pair[i].first->getMatId() != pair[i].second->getMatId()) {
            throw ErrorInvalidBoundary();
        }
        res.push_back(new HexI8(coords(), id, box, layId, matId));
    }
    return res;
}

vector<pair<const SurfI*, const SurfI*> >
MeshStructured::getPairsDefiningVolumeWithin(
        const GroupElements<const SurfI>& origBound) const {
    vector<pair<const SurfI*, const SurfI*> > res;
    const UInt nOrigBound = origBound.size();
    // Checks that bound.size is an even number.
    if (nOrigBound % 2 != 0) {
        throw ErrorInvalidBoundary();
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

Real MeshStructured::getMinimumSpaceStep() const {
    return Grid3::getMinimumSpaceStep();
}

BoxR3 MeshStructured::getBoundingBox() const {
    return Grid3::getFullDomainBoundingBox();
}
