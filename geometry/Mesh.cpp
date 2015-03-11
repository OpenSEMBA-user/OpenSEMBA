/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "Mesh.h"

const Real Mesh::areaDiffTolerance = 1e-15;

Mesh::Mesh() {
	grid_ = NULL;
}

Mesh::Mesh(
        const CoordinateGroup<>& cG,
        const Grid3* grid) {
    cG_ = cG;
    if (grid != NULL) {
        grid_ = new Grid3(*grid);
    } else {
        grid_ = NULL;
    }
}

Mesh::Mesh(
        const CoordinateGroup<>& cG,
        const ElementsGroup<>& elem,
        const Grid3* grid)
: ElementsGroup<>(elem) {
    cG_ = cG;
    reassignPointers(cG_);
    if (grid != NULL) {
        grid_ = new Grid3(*grid);
    } else {
        grid_ = NULL;
    }
}

Mesh::Mesh(Mesh& rhs) {
    cG_ = rhs.cG_;
    ElementsGroup<>::operator=(rhs);
    reassignPointers(cG_);
    map_ = rhs.map_;
    map_.reassignPointers(*this);
    if (rhs.grid_ != NULL) {
        grid_ = new Grid3(*rhs.grid_);
    } else {
        grid_ = NULL;
    }
}

Mesh::~Mesh() {

}


Face
Mesh::getBoundary(const SurfR* surf) const {
    return map_.getInnerFace(surf->getId());
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
        res.add(getPtrToId(ElementId(aux(i,0))));
    }
    return res;
}

Face Mesh::getNeighConnection(Face inner) const {
    UInt inId = inner.first->getId();
    UInt inFace = inner.second;
    return map_.getNeighConnection(inId, inFace);
}

bool
Mesh::isFloatingCoordinate(const CoordR3* param) const {
    for (UInt i = 0; i < element_.size(); i++) {
        if(!element_[i]->is<ElementBase>())
            continue;
        const ElementBase* elem = element_[i]->castTo<ElementBase>();
        for (UInt j = 0; j < elem->numberOfCoordinates(); j++) {
            if (*param == *elem->getV(j)) {
                return false;
            }
        }
    }
    return true;
}

vector<BoxR3>
Mesh::getRectilinearHexesInsideRegion(
        const ElementsGroup<ElemR>& region) const {
    // Determines positions to query.
    vector<CVecR3> center =
            grid_->getCenterOfNaturalCellsInside(region.getBound());
    // Determines if positions are inside tetrahedrons.
    vector<BoxR3> res;
    res.reserve(center.size());
    for (UInt i = 0; i < center.size(); i++) {
        for (UInt j = 0; j < region.size(); j++) {
            if (region(j)->castTo<VolR>()->isInnerPoint(center[i])) {
                res.push_back(grid_->getBoundingBoxContaining(center[i]));
                break;
            }
        }
    }
    return res;
}

bool
Mesh::isOnBoundary(const CVecR3 pos) const {
#warning "Not implemented"
}

ElementsGroup<SurfR> Mesh::getMaterialBoundary(const MatId   matId,
                                               const LayerId layId) const {
    return get(ElementBase::surface, matId, layId).getGroupOf<SurfR>();
}

vector<BoxR3> Mesh::discretizeWithinBoundary(
        const UInt matId,
        const UInt layId) const {
#warning "Not implemented"
}

ElementsGroup<Tri3>
Mesh::getTriWithMatId(
        MatId matId,
        bool ignoreTet) const {
    vector<Tri3*> res;
    ElementsGroup<Tri3> tri3 = getGroupOf<Tri3>();
    for (UInt i = 0; i < tri3.size(); i++) {
        if (tri3(i)->getMatId() == matId) {
            res.push_back(new Tri3(*tri3(i)));
        }
    }
    ElementsGroup<Tri6> tri6 = getGroupOf<Tri6>();
    for (UInt i = 0; i < tri6.size(); i++) {
        if (tri6(i)->getMatId() == matId) {
            res.push_back(tri6(i)->linearize());
        }
    }
    if (!ignoreTet) {
        ElementsGroup<Tet> tet = getGroupOf<Tet>();
        ElementsGroup<> region;
        for (UInt i = 0; i < tet.size(); i++) {
            if (tet(i)->getMatId() == matId) {
                region.add(tet(i)->castTo<ElementBase>());
            }
        }
        vector<Face> internalBorder = getInternalBorder(region);
        for (UInt i = 0; i < internalBorder.size(); i++) {
            const VolR* vol = internalBorder[i].first;
            const UInt face = internalBorder[i].second;
            res.push_back(vol->castTo<Tet>()->getTri3Face(face));
        }
    }
    return ElementsGroup<Tri3>(res);
}

vector<Face>
Mesh::getInternalBorder(const ElementsGroup<>& region) const {
    // Runs over all elements contained in the region vector detecting
    // the internal border. Returns a vector containing the element
    // faces composing the internal border.
    vector<Face> tri, res;
    res = getInternalBorder(region.getGroupOf<Tet>());
    tri = getInternalBorder(region.getGroupOf<Tri>());
    res.insert(res.end(), tri.begin(), tri.end());
    return res;
}

vector<Face>
Mesh::getExternalBorder(const ElementsGroup<>& region) const {
    // Generates a vector of pairs pointers to tetrahedrons and faces that
    // connects with the region specified by the elemIds inputted.
    // If the element's face in the inputted region is in the computational
    // border nothing is returned.
    vector<Face> internal = getInternalBorder(region);
    vector<Face> external;
    external.reserve(internal.size());
    for (UInt i = 0; i < internal.size(); i++) {
        UInt inId = internal[i].first->getId();
        UInt inFace = internal[i].second;
        const VolR* outVol = map_.getNeighbour(inId, inFace);
        UInt outFace = map_.getVolToF(inId, inFace);
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
            if(!getPtrToId(ElementId(fList(k,0)))->is<Tet>())
                continue;
            const Tet* tet = getPtrToId(ElementId(fList(k,0)))->castTo<Tet>();
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
    for (UInt i = 0; i < nE; i++) {
        res[i] = getBoundary(region(i)->castTo<Tri>());
    }
    return res;
}

bool
Mesh::isRectilinear() const {
	bool hasCartesianGridDefined = (grid_ != NULL);
    bool onlyContainsQuad4 = (sizeOf<QuadR4>() == size());
	return (hasCartesianGridDefined && onlyContainsQuad4);
}

const Grid3*
Mesh::getGrid() const {
	return grid_;
}

void
Mesh::setGrid(const Grid3& grid) {
	grid_ = new Grid3(grid);
}

void
Mesh::applyGeometricScalingFactor(
 const Real factor) {
	cG_.applyScalingFactor(factor);
	if (grid_ != NULL) {
		grid_->applyScalingFactor(factor);
	}
}

vector<pair<const ElemR*, UInt> >
Mesh::getElementsWithVertex(const UInt vertexId,
                            const ElementBase::Type type) const {

    vector<pair<const ElemR*, UInt> > res;
    for (UInt i = 0; i < element_.size(); i++) {
        if(!element_[i]->is<ElemR>())
            continue;
        const ElemR* e = element_[i]->castTo<ElemR>();
        for (UInt j = 0; j < e->numberOfVertices(); j++) {
            if (e->getType() == type && e->getVertex(j)->getId() == vertexId) {
                pair<const ElemR*, UInt> aux(e,j);
                res.push_back(aux);
            }
        }
    }
    return res;
}


vector<ElementId> Mesh::addAsHex8(const BoxR3& box) {
    cG_.add(box.getPos());
    vector<HexR8> hexes;
    hexes.push_back(HexR8(cG_, ElementId(0), box.getMin(), box.getMax()));
    return add(cG_, hexes);
}

void Mesh::printInfo() const {
    cout << " --- Mesh Info --- " << endl;
    cG_.printInfo();
    ElementsGroup<>::printInfo();
    if (grid_ != NULL) {
        grid_->printInfo();
    }
}
