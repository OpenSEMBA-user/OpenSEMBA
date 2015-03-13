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

Mesh::Mesh(
        const CoordinateGroup<>& cG,
        const ElementsGroup<>& elem,
        const LayerGroup<>& layers)
:  LayerGroup<>(layers), ElementsGroup<>(elem) {
    cG_ = cG;
    reassignPointers(cG_);
}

Mesh::Mesh(Mesh& rhs) {
    cG_ = rhs.cG_;
    ElementsGroup<>::operator=(rhs);
    reassignPointers(cG_);
}

Mesh::~Mesh() {

}

vector<Face>
Mesh::getBorderWithNormal(
        const vector<Face>& border,
        const CVecR3& normal) {
    const UInt nK = border.size();
    vector<Face> res;
    res.reserve(nK);
    for (UInt i = 0; i < nK; i++) {
        const VolR * tet = border[i].first;
        const UInt face = border[i].second;
        CartesianVector<Real,3> tetNormal = tet->sideNormal(face);
        if (tetNormal == normal && !tet->isCurvedFace(face)) {
            res.push_back(border[i]);
        }
    }
    return res;
}

ElementsGroup<Tri>*
Mesh::convertToTri(const ElementsGroup<>& region, bool includeTets) const {
    ElementsGroup<Tri>* res = new ElementsGroup<Tri>(region.newGroupOf<Tri>());
    if (includeTets) {
        ElementsGroup<Tet> tet = region.getGroupOf<Tet>();
        vector<Face> border = getInternalBorder(tet);
        for (UInt i = 0; i < border.size(); i++) {
            const VolR* vol = border[i].first;
            const UInt face = border[i].second;
            res->add(vol->castTo<Tet>()->getTri3Face(face));
        }
    }
    return res;
}

vector<Face>
Mesh::getInternalBorder(const ElementsGroup<>& region) const {
    vector<Face> tri, res;
    res = getInternalBorder(region.getGroupOf<Tet>());
    tri = getInternalBorder(region.getGroupOf<Tri>());
    res.insert(res.end(), tri.begin(), tri.end());
    return res;
}

vector<Face>
Mesh::getExternalBorder(const ElementsGroup<>& region) const {
    vector<Face> internal = getInternalBorder(region);
    vector<Face> external;
    const MapGroup mapGroup(cG_, region);
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

vector<Face>
Mesh::getInternalBorder(const ElementsGroup<Tet>& region) const {
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
            if(!ElementsGroup<>::getPtrToId(ElementId(fList(k,0)))->is<Tet>())
                continue;
            const Tet* tet = ElementsGroup<>::getPtrToId(ElementId(fList(k,0)))->castTo<Tet>();
            UInt face = fList(k,1);
            Face aux(tet, face);
            res.push_back(aux);
        }
    }
    return res;
}

vector<Face>
Mesh::getInternalBorder(const ElementsGroup<Tri>& region) const {
    UInt nE = region.size();
    vector<Face> res(nE);
    MapGroup mapGroup(cG_, region.getGroupOf<ElementBase>());
    for (UInt i = 0; i < nE; i++) {
        res[i] = mapGroup.getInnerFace(region(i)->getId());
    }
    return res;
}

ElementsGroup<>
Mesh::getAdjacentRegion(const ElementsGroup<>& region) {
    vector<Face> outer = getExternalBorder(region);
    UInt nOut = outer.size();
    // Removes repeated.
    DynMatrix<UInt> aux(nOut,1);
    for (UInt i = 0; i < nOut; i++) {
        aux(i,0) = outer[i].first->getId();
    }
    aux.sortAndRemoveRepeatedRows_omp();
    // Prepares result.
    ElementsGroup<> res;
    for (UInt i = 0; i < aux.nRows(); i++) {
        res.add(ElementsGroup<>::getPtrToId(ElementId(aux(i,0))));
    }
    return res;
}

bool
Mesh::isFloatingCoordinate(const CoordR3* param) const {
    for (UInt i = 0; i < ElementsGroup<>::size(); i++) {
        const ElementBase* elem = ElementsGroup<>::operator()(i);
        for (UInt j = 0; j < elem->numberOfCoordinates(); j++) {
            if (*param == *elem->getV(j)) {
                return false;
            }
        }
    }
    return true;
}

bool
Mesh::isOnBoundary(const CVecR3 pos) const {
#warning "Not implemented"
}

ElementsGroup<SurfR>
Mesh::getMaterialBoundary(const MatId matId, const LayerId layId) const {
    return ElementsGroup<>::get(matId, layId).getGroupOf<SurfR>();
}

void
Mesh::applyScalingFactor(
        const Real factor) {
    cG_.applyScalingFactor(factor);
}

vector<ElementId> Mesh::addAsHex8(const BoxR3& box) {
    cG_.add(box.getPos());
    vector<HexR8> hexes;
    hexes.push_back(HexR8(cG_, ElementId(0), box.getMin(), box.getMax()));
    return ElementsGroup<>::add(cG_, hexes);
}

void Mesh::printInfo() const {
    cout << " --- Mesh Info --- " << endl;
    cG_.printInfo();
    ElementsGroup<>::printInfo();
}
