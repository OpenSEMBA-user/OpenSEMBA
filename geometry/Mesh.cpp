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
        const Grid3* grid) {
    cG_ = cG;
    elem_ = elem;
    elem_.reassignPointers(cG_);
    if (grid != NULL) {
        grid_ = new Grid3(*grid);
    } else {
        grid_ = NULL;
    }
}

Mesh::Mesh(Mesh& param) {
    cG_ = param.cG_;
    elem_ = param.elem_;
    elem_.reassignPointers(cG_);
//    map_ = param.map_;
//    map_.reassignPointers(elem_);
    if (param.grid_ != NULL) {
        grid_ = new Grid3(*param.grid_);
    } else {
        grid_ = NULL;
    }
}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}


//pair<const Volume*, UInt>
//Mesh::getBoundary(const Surface* surf) const {
//    return map_.getInnerFace(surf->getId());
//}

vector<pair<const Tet*, UInt> >
Mesh::getBorderWithNormal(
 const vector<pair<const Tet*, UInt> >& border,
 const CartesianVector<Real, 3>& normal) {
    const UInt nK = border.size();
    vector<pair<const Tet*, UInt> > res;
    res.reserve(nK);
    for (UInt i = 0; i < nK; i++) {
        const Tet* tet = border[i].first;
        const UInt face = border[i].second;
        CartesianVector<Real,3> tetNormal = tet->sideNormal(face);
        if (tetNormal == normal && !tet->isCurvedFace(face)) {
            res.push_back(border[i]);
        }
    }
    return res;
}

//vector<UInt>
//Mesh::getAdjacentElements(const vector<ElementId>& region) const {
//    vector<pair<const Volume*, UInt> > outer;
//    outer = getExternalBorder(region);
//    UInt nOut = outer.size();
//    // Removes repeated.
//    DynMatrix<UInt> aux(nOut,1);
//    for (UInt i = 0; i < nOut; i++) {
//        aux(i,0) = outer[i].first->getId();
//    }
//    aux.sortAndRemoveRepeatedRows_omp();
//    // Prepares result.
//    vector<UInt> res(aux.nRows(), 0);
//    for (UInt i = 0; i < aux.nRows(); i++) {
//        res[i] = aux(i,0);
//    }
//    return res;
//}

//pair<const Volume*, UInt> Mesh::getNeighConnection(
//        pair<const Volume*, const UInt> inner) const {
//    UInt inId = inner.first->getId();
//    UInt inFace = inner.second;
//    return map_.getNeighConnection(inId, inFace);
//}

bool
Mesh::isFloatingCoordinate(const CoordR3* param) const {
    for (UInt i = 0; i < elem_.size(); i++) {
        if(!elem_(i)->is<ElemR>())
            continue;
        const ElemR* elem = elem_(i)->castTo<ElemR>();
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
    vector<ElementId> ids = region.getIds();
    BoxR3 bound(getBound(ids));
    vector<CVecR3> center = grid_->getCenterOfNaturalCellsInside(bound);
    // Determines if positions are inside tetrahedrons.
    vector<BoxR3> res;
    res.reserve(center.size());
    for (UInt i = 0; i < center.size(); i++) {
        for (UInt j = 0; j < region.size(); j++) {
            if (region(j)->isInnerPoint(center[i])) {
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

const CoordR3* Mesh::getClosestVertex(const CVecR3 pos) const {
    const CoordR3* res;
    Real minDist = numeric_limits<Real>::infinity();
    for (UInt b = 0; b < elem_.size(); b++) {
        if(!elem_(b)->is<ElemR>())
            continue;
        const ElemR* element = elem_(b)->castTo<ElemR>();
        for (UInt i = 0; i < element->numberOfCoordinates(); i++) {
            const CoordR3* candidate = element->getV(i);
            if ((candidate->pos() - res->pos()).norm() < minDist) {
                res = candidate;
            }
        }
    }
    return res;
}

ElementsGroup<SurfR> Mesh::getMaterialBoundary(
        const MatId   matId,
        const LayerId layId) const {
    return elem_.get(matId, layId).getGroupOf<SurfR>();
}

vector<BoxR3> Mesh::discretizeWithinBoundary(
        const UInt matId,
        const UInt layId) const {
#warning "Not implemented"
}


void
Mesh::setMaterialIds(
 const vector<ElementId>& id,
 const MatId newMatId) {
	const UInt nIds = id.size();
	for (UInt i = 0; i < nIds; i++) {
	    elem_.getPtrToId(id[i])->setMatId(newMatId);
	}
}

vector<UInt>
Mesh::getIdsOfCurvedTets() const {
    vector<UInt> res;
    ElementsGroup<Tet> tet = elem_.getGroupOf<Tet>();
    UInt nK = tet.size();
    for (UInt k = 0; k < nK; k++) {
        if (tet(k)->isCurved()) {
            res.push_back(tet(k)->getId());
        }
    }
    return res;
}


ElementsGroup<Tri3>
Mesh::getTriWithMatId(
        const UInt matId,
        const bool ignoreTet) const {
    vector<Tri3*> elems;
    const UInt nTri = elem_.sizeOf<Tri>();
    const UInt nTet = elem_.sizeOf<Tet>();
    elems.reserve(nTri + nTet);
    // --- Runs over surfaces -------------------------------------------------
    ElementsGroup<Tri3> tri3 = elem_.getGroupOf<Tri3>();
    const UInt nTri3 = tri3.size();
    for (UInt i = 0; i < nTri3; i++) {
        if (tri3(i)->getMatId() == matId) {
        	elems.push_back(tri3(i)->linearize());
        }
    }
    ElementsGroup<Tri6> tri6 = elem_.getGroupOf<Tri6>();
    const UInt nTri6 = tri6.size();
    for (UInt i = 0; i < nTri6; i++) {
        if (tri6(i)->getMatId() == matId) {
        	elems.push_back(tri6(i)->linearize());
        }
    }
    // --- Runs over tetrahedrons ---------------------------------------------
//    if (!ignoreTet) {
//    	ElementsGroup<Tet> tet = elem_.getGroupOf<Tet>();
//        vector<ElementId> tetIds;
//        tetIds.reserve(nTet);
//        for (UInt i = 0; i < nTet; i++) {
//            // Generates list of tetrahedrons ids.
//            if (tet(i)->getMatId() == matId) {
//                tetIds.push_back(tet(i)->getId());
//            }
//        }
//        // Gets internal border of tetrahedron volume.
//        vector<pair<const Volume*, UInt> > internalBorder;
//        internalBorder = getInternalBorder(tetIds);
//        // Converts internal border to Tri3.
//        for (UInt i = 0; i < internalBorder.size(); i++) {
//            const Volume* vol = internalBorder[i].first;
//            const Tet* tet = dynamic_cast<const Tet*>(vol);
//            const UInt face = internalBorder[i].second;
//            elems.push_back(tet->getTri3Face(face));
//        }
//    }
    return ElementsGroup<Tri3>(elems);
}


ElementsGroup<Tri3>
Mesh::getTriWithId(const vector<ElementId>& ids) const {
    vector<Tri3*> elems;
    ElementsGroup<Tri3> tri3 = elem_.getGroupOf<Tri3>();
    elems.reserve(tri3.size());
    for (UInt i = 0; i < ids.size(); i++) {
        for (UInt j = 0; j < tri3.size(); j++) {
            if (ids[i] == tri3(j)->getId()) {
            	elems.push_back(tri3(j)->linearize());
            }
        }
    }
    return ElementsGroup<Tri3>(elems);
}

vector<ElementId>
Mesh::getTetIds(
 const vector<ElementId> elemIds) const {
    vector<ElementId> res;
    res.reserve(elemIds.size());
    const UInt nId = elemIds.size();
    for (UInt i = 0; i < nId; i++) {
        if (elem_.getPtrToId(elemIds[i])->is<Tet>()) {
            res.push_back(elemIds[i]);
        }
    }
    return res;
}

//vector<pair<const Volume*, UInt> >
//Mesh::getInternalBorder(
// const vector<ElementId>& region) const {
//    // Runs over all elements contained in the region vector detecting
//    // the internal border. Returns a vector containing the element
//    // faces composing the internal border.
//    if (elem_.areTetrahedrons(region)) {
//        return getInternalBorderOfTetRegion(region);
//    } else if (elem_.areTriangles(region)) {
//        return getInternalBorderOfTriRegion(region);
//    }
//    cerr << endl << "ERROR @ getInternalBorder: "
//      << "Unable to detect region." << endl;
//    return vector<pair<const Volume*, UInt> >();
//}

//vector<pair<const Volume*, UInt> >
//Mesh::getExternalBorder(
// const vector<ElementId>& elemIds) const {
//    // Generates a vector of pairs pointers to tetrahedrons and faces that
//    // connects with the region specified by the elemIds inputted.
//    // If the element's face in the inputted region is in the computational
//    // border nothing is returned.
//    vector<pair<const Volume*, UInt> > internal;
//    internal = getInternalBorder(elemIds);
//    UInt nI = internal.size();
//    vector<pair<const Volume*, UInt> > external;
//    external.reserve(nI);
//    for (UInt i = 0; i < nI; i++) {
//        UInt inId = internal[i].first->getId();
//        UInt inFace = internal[i].second;
//        const Tet* outVol = map_.getNeighbour(inId, inFace);
//        UInt outFace = map_.getVolToF(inId, inFace);
//        if (outVol->getId() != inId || inFace != outFace)  {
//            pair<const Volume*, UInt> aux(outVol, outFace);
//            external.push_back(aux);
//        }
//    }
//    return external;
//}


//vector<pair<const Volume*, UInt> >
//Mesh::getInternalBorderOfTetRegion(
// const vector<ElementId>& region) const {
//    // Builds a list with all tetrahedron faces.
//    static const UInt faces = 4;
//    static const UInt nVert = 3;
//    UInt nK = region.size();
//    UInt nList = nK * faces;
//    DynMatrix<UInt> fList(nList, 2 + nVert);
//    for (UInt k = 0; k < nK; k++) {
//        if(!elem_.getPtrToId(region[k])->is<Tet>())
//            continue;
//        const Tet* tet = elem_.getPtrToId(region[k])->castTo<Tet>();
//        for (UInt f = 0; f < faces; f++) {
//            UInt row = k * faces + f;
//            fList(row, 0) = tet->getId();
//            fList(row, 1) = f;
//            UInt ordered[nVert];
//            tet->getOrderedSideVerticesId(ordered, f);
//            for (UInt i = 0; i < nVert; i++) {
//                fList(row, i + 2) = ordered[i];
//            }
//        }
//    }
//    // Sorts according to the coordinates id. Pairing matching faces. ---------
//    // If there is not a pair, then that element is not connected with other
//    // remaining connected with itself.
//    fList.sortRows_omp(2,4);
//    // Copies non repeated faces into result vector.
//    vector<pair<const Volume*, UInt> > res;
//    res.reserve(nK);
//    bool matches;
//    for (UInt k = 0; k < nList; k++) {
//        if (k < nList-1) {
//            matches = fList(k, 2) == fList(k+1, 2);
//            matches &= fList(k, 3) == fList(k+1, 3);
//            matches &= fList(k, 4) == fList(k+1, 4);
//        } else {
//            matches = false;
//        }
//        if (matches) {
//            k++;
//        } else {
//            if(!elem_.getPtrToId(ElementId(fList(k,0)))->is<Tet>())
//                continue;
//            const Tet* tet = elem_.getPtrToId(
//                                 ElementId(fList(k,0)))->castTo<Tet>();
//            UInt face = fList(k,1);
//            pair<const Tet*, UInt> aux(tet, face);
//            res.push_back(aux);
//        }
//    }
//    return res;
//}

//vector<pair<const Volume*, UInt> >
//Mesh::getInternalBorderOfTriRegion(
// const vector<ElementId>& region) const {
//    UInt nE = region.size();
//    vector<pair<const Volume*, UInt> > res(nE);
//    for (UInt i = 0; i < nE; i++) {
//       const Surface* surf =
//        dynamic_cast<const Surface*>(elem_.getPtrToId(region[i]));
//        res[i] = getBoundary(surf);
//    }
//    return res;
//}

bool
Mesh::isLinear() const {
	return elem_.isLinear();
}

void
Mesh::linearize() {
	elem_.linearize();
}

BoxR3
Mesh::getBound(const vector<ElementId>& list) const {
	// Inits bounding box.
	if (list.size() == 0) {
	   return BoxR3().setInfinity();
	}
	// Runs over border computing the bounding box of each face.
	if(!elem_.getPtrToId(list[0])->is<ElemR>())
	    return BoxR3();
	const ElemR* elem = elem_.getPtrToId(list[0])->castTo<ElemR>();
	BoxR3 bound = elem->getBound();
	const UInt nK = list.size();
	for (UInt i = 1; i < nK; i++) {
	    if(!elem_.getPtrToId(list[i])->is<ElemR>())
            continue;
	    const ElemR* elem = elem_.getPtrToId(list[i])->castTo<ElemR>();
		bound << elem->getBound();
	}
	return bound;
}

BoxR3
Mesh::getBound(
 const vector<pair<const Volume<Real>*, UInt> >& border) const {
	// Inits bounding box.
   if (border.size() == 0) {
      return BoxR3().setInfinity();
   }
	const Volume<Real>* tet = border[0].first;
	const UInt face = border[0].second;
	BoxR3 bound = tet->getBoundOfFace(face);
	// Runs over border computing the bounding box of each face.
	for (UInt i = 1; i < border.size(); i++) {
		const Volume<Real>* vol = border[i].first;
		const UInt face = border[i].second;
		bound << vol->getBoundOfFace(face);
	}
	return bound;
}

vector<ElementId>
Mesh::getIdsInsideBound(const BoxR3& bound, const ElementBase::Type type) const {
    const UInt nK = elem_.size();
    vector<ElementId> res;
    res.reserve(nK);
    BoxR3 localBound;
    for (UInt i = 0; i < nK; i++) {
        if (!elem_(i)->is<ElemR>())
            continue;
        const ElemR* e = elem_(i)->castTo<ElemR>();
        localBound = e->getBound();
        if (localBound <= bound
        && (e->getType() == type || type==ElementBase::undefined)) {
            res.push_back(e->getId());
        }
    }
    return res;
}

bool
Mesh::isRectilinear() const {
	bool hasCartesianGridDefined = (grid_ != NULL);
	bool onlyContainsQuad4 = (elem_.sizeOf< Quad4<Real> >() == elem_.size());
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
    for (UInt i = 0; i < elem_.size(); i++) {
        if(!elem_(i)->is<ElemR>())
            continue;
        const ElemR* e = elem_(i)->castTo<ElemR>();
        for (UInt j = 0; j < e->numberOfVertices(); j++) {
            if (e->getType() == type && e->getVertex(j)->getId() == vertexId) {
                pair<const ElemR*, UInt> aux(e,j);
                res.push_back(aux);
            }
        }
    }
    return res;
}

void Mesh::printInfo() const {
    cout << " --- Mesh Info --- " << endl;
    cG_.printInfo();
    elem_.printInfo();
    if (grid_ != NULL) {
        grid_->printInfo();
    }
}

vector<ElementId> Mesh::addAsHex8(const BoxR3& box) {
    cG_.add(box.getPos());
    vector<HexR8> hexes;
    hexes.push_back(HexR8(cG_, ElementId(0), box.getMin(), box.getMax()));
    return elem_.add(cG_, hexes);
}
