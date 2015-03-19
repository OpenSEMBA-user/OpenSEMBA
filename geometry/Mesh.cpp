/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "Mesh.h"

const Real Mesh::areaDiffTolerance = 1e-15;

Mesh::Mesh() {

}

Mesh::Mesh(const CoordinateGroup<CoordR3>& cG,
           const ElementsGroup<ElemR>& elem,
           const LayerGroup<>& layers)
:   CoordinateGroup<CoordR3>(cG.newGroupOf<CoordR3>()),
    ElementsGroup<ElemR>(elem.newGroupOf<ElemR>()),
    LayerGroup<>(layers.newGroupOf<Layer>()) {

    ElementsGroup<ElemR>::reassignPointers(*this);
}

Mesh::Mesh(const Mesh& rhs)
:   CoordinateGroup<CoordR3>(rhs.coords().newGroupOf<CoordR3>()),
    ElementsGroup<ElemR>(rhs.elems().newGroupOf<ElemR>()),
    LayerGroup<>(rhs.layers().newGroupOf<Layer>()) {

    ElementsGroup<ElemR>::reassignPointers(*this);
}

Mesh::~Mesh() {

}

Mesh& Mesh::operator=(const Mesh& rhs) {
    if(this == &rhs) {
        return *this;
    }

    CoordinateGroup<CoordR3>::operator=(rhs.coords().newGroupOf<CoordR3>());
    ElementsGroup<ElemR>::operator=(rhs.elems().newGroupOf<ElemR>());
    LayerGroup<>::operator=(rhs.layers().newGroupOf<Layer>());

    ElementsGroup<ElemR>::reassignPointers(*this);

    return *this;
}

vector<Face> Mesh::getBorderWithNormal(const vector<Face>& border,
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

ElementsGroup<Tri> Mesh::convertToTri(
        const ElementsGroup<ElemR>& region, bool includeTets) const {

    ElementsGroup<Tri> res = region.newGroupOf<Tri>();
    if (includeTets) {
        ElementsGroup<Tet> tet = region.getGroupOf<Tet>();
        vector<Face> border = getInternalBorder(tet);
        for (UInt i = 0; i < border.size(); i++) {
            if (border[i].first->is<Tet>()) {
                const Tet* tet = border[i].first->castTo<Tet>();
                const UInt face = border[i].second;
                res.add(tet->getTri3Face(face));
            }
        }
    }
    return res;
}

vector<Face> Mesh::getInternalBorder(
        const ElementsGroup<ElemR>& region) const {

    vector<Face> tri, res;
    res = getInternalBorder(region.getGroupOf<Tet>());
    tri = getInternalBorder(region.getGroupOf<Tri>());
    res.insert(res.end(), tri.begin(), tri.end());
    return res;
}

vector<Face> Mesh::getExternalBorder(
        const ElementsGroup<ElemR>& region) const {

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

vector<Face> Mesh::getInternalBorder(
        const ElementsGroup<Tet>& region) const {

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
            UInt ordered[nVert];
            region(k)->castTo<Tet>()->getOrderedSideVerticesId(ordered, f);
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
            if(!ElementsGroup<ElemR>::getPtrToId(ElementId(fList(k,0)))->
                    is<Tet>())
                continue;
            const Tet* tet =
                ElementsGroup<ElemR>::getPtrToId(ElementId(fList(k,0)))->
                    castTo<Tet>();
            UInt face = fList(k,1);
            Face aux(tet, face);
            res.push_back(aux);
        }
    }
    return res;
}

vector<Face> Mesh::getInternalBorder(
        const ElementsGroup<Tri>& region) const {

    UInt nE = region.size();
    vector<Face> res(nE);
    MapGroup mapGroup(CoordinateGroup<CoordR3>::getGroupOf<Coord>(),
                      region.getGroupOf<Elem>());
    for (UInt i = 0; i < nE; i++) {
        res[i] = mapGroup.getInnerFace(region(i)->getId());
    }
    return res;
}

ElementsGroup<ElemR> Mesh::getAdjacentRegion(
        const ElementsGroup<ElemR>& region) {
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
        res.add(ElementsGroup<ElemR>::getPtrToId(ElementId(aux(i,0)))->
                    clone()->castTo<ElemR>());
    }
    return res;
}

vector<BoxR3> Mesh::getRectilinearHexesInsideRegion(
        const Grid3* grid,
        const ElementsGroup<VolR>& region) const {

    vector<CVecR3> center =
        grid->getCenterOfCellsInside(region.getBound());
    vector<BoxR3> res;
    res.reserve(center.size());
    for (UInt i = 0; i < center.size(); i++) {
        for (UInt j = 0; j < region.size(); j++) {
            if (region(j)->isInnerPoint(center[i])) {
                res.push_back(grid->getBoundingBoxContaining(center[i]));
                break;
            }
        }
    }
    return res;
}

bool Mesh::isFloatingCoordinate(const CoordR3* param) const {
    ElementsGroup<ElemR> elems =
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

bool Mesh::isOnBoundary(const CVecR3 pos) const {
#warning "Not implemented"
}

ElementsGroup<SurfR> Mesh::getMaterialBoundary(
        const MatId matId,
        const LayerId layId) const {

    return ElementsGroup<ElemR>::get(matId, layId).
               getGroupOf<SurfR>();
}

vector<ElementId> Mesh::addAsHex8(const BoxR3& box) {
    vector<ElementId> res;
    Hex8<Real>* hex = new Hex8<Real>(*this, ElementId(0), box);
    ElementsGroup<ElemR>::add(hex->castTo<ElemR>(), true);
    res.push_back(hex->getId());
    return res;
}

void Mesh::printInfo() const {
    cout << " --- Mesh Info --- " << endl;
    CoordinateGroup<CoordR3>::printInfo();
    ElementsGroup<ElemR>::printInfo();
    LayerGroup<>::printInfo();
}
