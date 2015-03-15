/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "Mesh.h"

template<typename E, typename C, typename L>
const Real Mesh<E,C,L>::areaDiffTolerance = 1e-15;

template<typename E, typename C, typename L>
Mesh<E,C,L>::Mesh() {

}

template<typename E, typename C, typename L>
Mesh<E,C,L>::Mesh(
        const CoordinateGroup<C>& cG,
        const ElementsGroup<E>& elem,
        const LayerGroup<L>& layers)
:   CoordinateGroup<C>(cG),
    ElementsGroup<E>(elem),
    LayerGroup<L>(layers) {

    ElementsGroup<E>::reassignPointers(*this);
}

template<typename E, typename C, typename L>
Mesh<E,C,L>::Mesh(const Mesh<E,C,L>& rhs)
:   CoordinateGroup<C>(rhs),
    ElementsGroup<E>(rhs),
    LayerGroup<L>(rhs) {

    ElementsGroup<E>::reassignPointers(*this);
}

template<typename E, typename C, typename L>
Mesh<E,C,L>::~Mesh() {

}

template<typename E, typename C, typename L>
Mesh<E,C,L>& Mesh<E,C,L>::operator=(const Mesh<E,C,L>& rhs) {
    if(this == &rhs) {
        return *this;
    }

    CoordinateGroup<C>::operator=(rhs);
    ElementsGroup<E>::operator=(rhs);
    LayerGroup<L>::operator=(rhs);

    ElementsGroup<E>::reassignPointers(*this);

    return *this;
}

template<typename E, typename C, typename L>
template<typename E2, typename C2, typename L2>
Mesh<E2,C2,L2> Mesh<E,C,L>::getMeshOf() {
    return Mesh<E2,C2,L2>(CoordinateGroup<C>::template getGroupOf<C2>(),
                          ElementsGroup<E>::template getGroupOf<E2>(),
                          LayerGroup<L>::template getGroupOf<L2>());
}

template<typename E, typename C, typename L>
template<typename E2, typename C2, typename L2>
Mesh<E2,C2,L2> Mesh<E,C,L>::newMeshOf() {
    return Mesh<E2,C2,L2>(CoordinateGroup<C>::template newGroupOf<C2>(),
                          ElementsGroup<E>::template newGroupOf<E2>(),
                          LayerGroup<L>::template newGroupOf<L2>());
}

template<typename E, typename C, typename L>
vector<Face> Mesh<E,C,L>::getBorderWithNormal(const vector<Face>& border,
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

template<typename E, typename C, typename L>
ElementsGroup<Tri>
    Mesh<E,C,L>::convertToTri(
        const ElementsGroup<E>& region, bool includeTets) const {

    ElementsGroup<Tri> res = region.template newGroupOf<Tri>();
    if (includeTets) {
        ElementsGroup<Tet> tet = region.template getGroupOf<Tet>();
        vector<Face> border = getInternalBorder(tet);
        for (UInt i = 0; i < border.size(); i++) {
            if (border[i].first->template is<Tet>()) {
                const Tet* tet = border[i].first->template castTo<Tet>();
                const UInt face = border[i].second;
                res.add(tet->getTri3Face(face));
            }
        }
    }
    return res;
}

template<typename E, typename C, typename L>
vector<Face> Mesh<E,C,L>::getInternalBorder(
        const ElementsGroup<E>& region) const {

    vector<Face> tri, res;
    res = getInternalBorder(region.template getGroupOf<Tet>());
    tri = getInternalBorder(region.template getGroupOf<Tri>());
    res.insert(res.end(), tri.begin(), tri.end());
    return res;
}

template<typename E, typename C, typename L>
vector<Face> Mesh<E,C,L>::getExternalBorder(
        const ElementsGroup<E>& region) const {

    vector<Face> internal = getInternalBorder(region);
    vector<Face> external;
    const MapGroup mapGroup(
        CoordinateGroup<C>::template getGroupOf<CoordinateBase>(),
        region.template getGroupOf<ElementBase>());
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

template<typename E, typename C, typename L>
vector<Face> Mesh<E,C,L>::getInternalBorder(
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
            if(!ElementsGroup<E>::getPtrToId(ElementId(fList(k,0)))->
                    template is<Tet>())
                continue;
            const Tet* tet =
                ElementsGroup<E>::getPtrToId(ElementId(fList(k,0)))->
                    template castTo<Tet>();
            UInt face = fList(k,1);
            Face aux(tet, face);
            res.push_back(aux);
        }
    }
    return res;
}

template<typename E, typename C, typename L>
vector<Face> Mesh<E,C,L>::getInternalBorder(
        const ElementsGroup<Tri>& region) const {

    UInt nE = region.size();
    vector<Face> res(nE);
    MapGroup mapGroup(*this, region.getGroupOf<ElementBase>());
    for (UInt i = 0; i < nE; i++) {
        res[i] = mapGroup.getInnerFace(region(i)->getId());
    }
    return res;
}

template<typename E, typename C, typename L>
ElementsGroup<E> Mesh<E,C,L>::getAdjacentRegion(
        const ElementsGroup<E>& region) {
    vector<Face> outer = getExternalBorder(region);
    UInt nOut = outer.size();
    // Removes repeated.
    DynMatrix<UInt> aux(nOut,1);
    for (UInt i = 0; i < nOut; i++) {
        aux(i,0) = outer[i].first->getId();
    }
    aux.sortAndRemoveRepeatedRows_omp();
    // Prepares result.
    ElementsGroup<E> res;
    for (UInt i = 0; i < aux.nRows(); i++) {
        res.add(ElementsGroup<E>::getPtrToId(ElementId(aux(i,0)))->
                    clone()->template castTo<ElementBase>());
    }
    return res;
}

template<typename E, typename C, typename L>
vector<BoxR3> Mesh<E,C,L>::getRectilinearHexesInsideRegion(
        const Grid3* grid,
        const ElementsGroup<VolR>& region) const {

    vector<CVecR3> center =
        grid->getCenterOfNaturalCellsInside(region.getBound());
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

template<typename E, typename C, typename L>
bool Mesh<E,C,L>::isFloatingCoordinate(const CoordR3* param) const {
    ElementsGroup<ElemR> elems =
        ElementsGroup<E>::template getGroupOf<ElemR>();
    for (UInt i = 0; i < elems.size(); i++) {
        for (UInt j = 0; j < elems(i)->numberOfCoordinates(); j++) {
            if (*param == *elems(i)->getV(j)) {
                return false;
            }
        }
    }
    return true;
}

template<typename E, typename C, typename L>
bool Mesh<E,C,L>::isOnBoundary(const CVecR3 pos) const {
#warning "Not implemented"
}

template<typename E, typename C, typename L>
ElementsGroup<SurfR> Mesh<E,C,L>::getMaterialBoundary(
        const MatId matId,
        const LayerId layId) const {

    return ElementsGroup<E>::get(matId, layId).
               template getGroupOf<SurfR>();
}

template<typename E, typename C, typename L>
vector<ElementId> Mesh<E,C,L>::addAsHex8(const BoxR3& box) {
    CoordinateGroup<C>::add(box.getPos());
    vector<HexR8> hexes;
    hexes.push_back(HexR8(*this, ElementId(0), box.getMin(), box.getMax()));
    return ElementsGroup<E>::add(*this, hexes);
}

template<typename E, typename C, typename L>
void Mesh<E,C,L>::printInfo() const {
    cout << " --- Mesh Info --- " << endl;
    CoordinateGroup<C>::printInfo();
    ElementsGroup<E>::printInfo();
    LayerGroup<L>::printInfo();
}
