/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "MeshStructured.h"
#include "MeshUnstructured.h"

MeshUnstructured::MeshUnstructured() {

}

MeshUnstructured::MeshUnstructured(const CoordinateGroup<const CoordR3>& cG,
        const ElementsGroup<const ElemR>& elem,
        const LayerGroup<const Layer>& layers)
:   CoordinateGroup<CoordR3>(cG.newGroup()),
    ElementsGroup<ElemR>(elem.newGroup()),
    LayerGroup<Layer>(layers.newGroup()) {

    ElementsGroup<ElemR>::reassignPointers(*this);
}

MeshUnstructured::MeshUnstructured(const MeshUnstructured& rhs)
:   CoordinateGroup<CoordR3>(rhs.coords().newGroup()),
    ElementsGroup<ElemR>(rhs.elems().newGroup()),
    LayerGroup<Layer>(rhs.layers().newGroup()) {

    ElementsGroup<ElemR>::reassignPointers(*this);
}

MeshUnstructured::~MeshUnstructured() {

}

MeshUnstructured& MeshUnstructured::operator=(const MeshUnstructured& rhs) {
    if(this == &rhs) {
        return *this;
    }

    CoordinateGroup<CoordR3>::operator=(rhs.coords().newGroup());
    ElementsGroup<ElemR>::operator=(rhs.elems().newGroup());
    LayerGroup<Layer>::operator=(rhs.layers().newGroup());

    ElementsGroup<ElemR>::reassignPointers(*this);

    return *this;
}

MeshStructured* MeshUnstructured::getMeshStructured(const Grid3& grid,
        const Real tol) const {
    MeshStructured* res = new MeshStructured(grid);

    vector<CoordI3*> newCoords;
    newCoords.reserve(coords().size());
    for (UInt i = 0; i < coords().size(); i++) {
        CoordI3* newCoord = coords()(i)->toStructured(grid);
        if (newCoord != NULL) {
            newCoords.push_back(newCoord);
        }
    }
    res->coords().add(newCoords);

    vector<ElemI*> newElems;
    newElems.reserve(elems().size());
    for (UInt i = 0; i < elems().size(); i++) {
        ElemI* newElem = elems()(i)->toStructured(*res, grid, tol);
        if (newElem != NULL) {
            newElems.push_back(newElem);
        }
    }
    res->elems().add(newElems);
    res->layers() = layers().newGroup();
    return res;
}
vector<Face> MeshUnstructured::getBorderWithNormal(const vector<Face>& border,
        const CVecR3& normal) {
    const UInt nK = border.size();
    vector<Face> res;
    res.reserve(nK);
    for (UInt i = 0; i < nK; i++) {
        const VolR* tet = border[i].first;
        const UInt face = border[i].second;
        CartesianVector<Real,3> tetNormal = tet->sideNormal(face);
        if (tetNormal == normal && !tet->isCurvedFace(face)) {
            res.push_back(border[i]);
        }
    }
    return res;
}

ElementsGroup<const Tri> MeshUnstructured::convertToTri(
        const ElementsGroup<const ElemR>& region,
        bool includeTets) const {

    ElementsGroup<Tri> res(region.newGroup());
    if (includeTets) {
        ElementsGroup<const Tet> tet = region.getGroupOf<Tet>();
        vector<Face> border = getInternalBorder(tet);
        for (UInt i = 0; i < border.size(); i++) {
            if (border[i].first->is<Tet>()) {
                const Tet* tet = border[i].first->castTo<Tet>();
                const UInt face = border[i].second;
                res.add(tet->getTri3Face(face), true);
            }
        }
    }
    return res;
}

vector<Face> MeshUnstructured::getInternalBorder(
        const ElementsGroup<const ElemR>& region) const {

    vector<Face> tri, res;
    res = getTetInternalBorder(region.getGroupOf<Tet>());
    tri = getTriInternalBorder(region.getGroupOf<Tri>());
    res.insert(res.end(), tri.begin(), tri.end());
    return res;
}

vector<Face> MeshUnstructured::getExternalBorder(
        const ElementsGroup<const ElemR>& region) const {

    vector<Face> internal = getInternalBorder(region);
    vector<Face> external;
    const MapGroup mapGroup(
            CoordinateGroup<CoordR3>::getGroupOf<Coord>(),
            region.getGroupOf<Elem>());
    external.reserve(internal.size());
    for (UInt i = 0; i < internal.size(); i++) {
        ElementId inId = internal[i].first->getId();
        UInt inFace = internal[i].second;
        const VolR* outVol = mapGroup.getNeighbour(inId, inFace);
        UInt outFace = mapGroup.getVolToF(inId, inFace);
        if (outVol->getId() != inId || inFace != outFace)  {
            external.push_back(Face(outVol, outFace));
        }
    }
    return external;
}

vector<Face> MeshUnstructured::getTetInternalBorder(
        const ElementsGroup<const Tet>& region) const {

    // Builds a list with all tetrahedron faces.
    static const UInt faces = 4;
    static const UInt nVert = 3;
    UInt nK = region.size();
    UInt nList = nK * faces;
    DynMatrix<UInt> fList(nList, 2 + nVert);
    for (UInt k = 0; k < nK; k++) {
        for (UInt f = 0; f < faces; f++) {
            UInt row = k * faces + f;
            fList(row, 0) = region(k)->getId();
            fList(row, 1) = f;
            vector<CoordinateId> ordered(nVert);
            ordered = ElementBase::getIds(region(k)->getSideVertices(f));
            ordered = ElementBase::ascendingIdOrder(ordered);
            for (UInt i = 0; i < nVert; i++) {
                fList(row, i + 2) = ordered[i];
            }
        }
    }
    // Sorts according to the coordinates id. Pairing matching faces. ---------
    // If there is not a pair, then that element is not connected with other
    // remaining connected with itself.
    fList.sortRows_omp(2,4);
    // Copies non repeated faces into result vector.
    vector<Face> res;
    res.reserve(nK);
    bool matches;
    for (UInt k = 0; k < nList; k++) {
        if (k < nList-1) {
            matches = fList(k, 2) == fList(k+1, 2);
            matches &= fList(k, 3) == fList(k+1, 3);
            matches &= fList(k, 4) == fList(k+1, 4);
        } else {
            matches = false;
        }
        if (matches) {
            k++;
        } else {
            if(!elems().get(ElementId(fList(k,0)))->is<Tet>())
                continue;
            const Tet* tet = elems().get(ElementId(fList(k,0)))->castTo<Tet>();
            UInt face = fList(k,1);
            Face aux(tet, face);
            res.push_back(aux);
        }
    }
    return res;
}

vector<Face> MeshUnstructured::getTriInternalBorder(
        const ElementsGroup<const Tri>& region) const {

    UInt nE = region.size();
    vector<Face> res(nE);
    MapGroup mapGroup(coords().getGroupOf<CoordR3>(), region);
    for (UInt i = 0; i < nE; i++) {
        res[i] = mapGroup.getInnerFace(region(i)->getId());
    }
    return res;
}

ElementsGroup<ElemR> MeshUnstructured::getAdjacentRegion(
        const ElementsGroup<const ElemR>& region) {
    vector<Face> outer = getExternalBorder(region);
    UInt nOut = outer.size();
    // Removes repeated.
    DynMatrix<UInt> aux(nOut,1);
    for (UInt i = 0; i < nOut; i++) {
        aux(i,0) = outer[i].first->getId();
    }
    aux.sortAndRemoveRepeatedRows_omp();
    // Prepares result.
    ElementsGroup<ElemR> res;
    for (UInt i = 0; i < aux.nRows(); i++) {
        res.add(elems().get(ElementId(aux(i,0)))->cloneTo<ElemR>());
    }
    return res;
}

bool MeshUnstructured::isFloatingCoordinate(const CoordR3* param) const {
    ElementsGroup<const ElemR> elems =
            ElementsGroup<ElemR>::getGroupOf<ElemR>();
    for (UInt i = 0; i < elems.size(); i++) {
        for (UInt j = 0; j < elems(i)->numberOfCoordinates(); j++) {
            if (*param == *elems(i)->getV(j)) {
                return false;
            }
        }
    }
    return true;
}

bool MeshUnstructured::isOnBoundary(const CVecR3 pos) const {
    // TODO Unstr isOnBoundary
}

ElementsGroup<const SurfR> MeshUnstructured::getMaterialBoundary(
        const MatId matId,
        const LayerId layId) const {

    return elems().getGroupWith(matId, layId).getGroupOf<SurfR>();
}

void MeshUnstructured::applyScalingFactor(const Real factor) {
    CoordinateGroup<CoordR3>::applyScalingFactor(factor);
}

void MeshUnstructured::printInfo() const {
    cout << " --- Mesh unstructured Info --- " << endl;
    CoordinateGroup<CoordR3>::printInfo();
    ElementsGroup<ElemR>::printInfo();
    LayerGroup<>::printInfo();
}

Real MeshUnstructured::getMinimumSpaceStep() const {
    // TODO Unstr minimum space step.
}

ElementsGroup<const SurfR> MeshUnstructured::getSurfsMatching(
        const vector<Face>& faces) const {
    vector<const SurfR*> res;
    IndexByVertexId index = getIndexByVertexId();
    for (UInt i = 0; i < faces.size(); i++) {
        const VolR* vol = faces[i].first;
        const UInt f = faces[i].second;
        vector<const CoordR3*> vertices = vol->getSideVertices(f);
        vector<CoordinateId> ids(vertices.size());
        for (UInt j = 0; j < vertices.size(); j++) {
            ids[j] = vertices[j]->getId();
        }
        IndexByVertexId::const_iterator it = index.find(ids);
        if (it == index.end()) {
            cerr << "ERROR @ MeshUnstrctured:"
                    << "Unable to find surf for face." << endl;
        }
        res.push_back(it->second->castTo<SurfR>());
    }
    return ElementsGroup<const SurfR>(res);
}





