/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "Mesh.h"

const double Mesh::areaDiffTolerance = 1e-15;

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
        const ElementsGroup& elem,
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
    map_ = param.map_;
    map_.reassignPointers(elem_);
    if (param.grid_ != NULL) {
        grid_ = new Grid3(*param.grid_);
    } else {
        grid_ = NULL;
    }
}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}


pair<const Volume*, uint>
Mesh::getBoundary(const Surface* surf) const {
    return map_.getInnerFace(surf->getId());
}

vector<pair<const Tet*, uint> >
Mesh::getBorderWithNormal(
 const vector<pair<const Tet*, uint> >& border,
 const CartesianVector<double, 3>& normal) {
    const uint nK = border.size();
    vector<pair<const Tet*, uint> > res;
    res.reserve(nK);
    for (uint i = 0; i < nK; i++) {
        const Tet* tet = border[i].first;
        const uint face = border[i].second;
        CartesianVector<double,3> tetNormal = tet->sideNormal(face);
        if (tetNormal == normal && !tet->isCurvedFace(face)) {
            res.push_back(border[i]);
        }
    }
    return res;
}

vector<uint>
Mesh::getAdjacentElements(const vector<uint>& region) const {
    vector<pair<const Volume*, uint> > outer;
    outer = getExternalBorder(region);
    uint nOut = outer.size();
    // Removes repeated.
    DynMatrix<uint> aux(nOut,1);
    for (uint i = 0; i < nOut; i++) {
        aux(i,0) = outer[i].first->getId();
    }
    aux.sortAndRemoveRepeatedRows_omp();
    // Prepares result.
    vector<uint> res(aux.nRows(), 0);
    for (uint i = 0; i < aux.nRows(); i++) {
        res[i] = aux(i,0);
    }
    return res;
}

pair<const Volume*, uint> Mesh::getNeighConnection(
        pair<const Volume*, const uint> inner) const {
    uint inId = inner.first->getId();
    uint inFace = inner.second;
    return map_.getNeighConnection(inId, inFace);
}

bool
Mesh::isFloatingCoordinate(const CoordD3* param) const {
    for (uint i = 0; i < elem_.element.size(); i++) {
        for (uint j = 0; j < elem_.element[i]->numberOfCoordinates(); j++) {
            if (*param == *elem_.element[i]->getV(j)) {
                return false;
            }
        }
    }
    return true;
}

vector<BoxD3>
Mesh::getRectilinearHexesInsideRegion(
        const vector<const Element*>& region) const {
    // Determines positions to query.
    vector<ElementId> ids = elem_.getIds(region);
    BoxD3 bound(getBound(ids));
    vector<CVecD3> center = grid_->getCenterOfNaturalCellsInside(bound);
    // Determines if positions are inside tetrahedrons.
    vector<BoxD3> res;
    res.reserve(center.size());
    for (uint i = 0; i < center.size(); i++) {
        for (uint j = 0; j < region.size(); j++) {
            if (region[j]->isInnerPoint(center[i])) {
                res.push_back(grid_->getBoundingBoxContaining(center[i]));
                break;
            }
        }
    }
    return res;
}

bool
Mesh::isOnBoundary(const CVecD3 pos) const {
#warning "Not implemented"
}

const CoordD3* Mesh::getClosestVertex(const CVecD3 pos) const {
    const CoordD3* res;
    double minDist = numeric_limits<double>::infinity();
    for (uint b = 0; b < elem_.element.size(); b++) {
        const Element* element = elem_.element[b];
        for (uint i = 0; i < element->numberOfCoordinates(); i++) {
            const CoordD3* candidate = element->getV(i);
            if ((candidate->pos() - res->pos()).norm() < minDist) {
                res = candidate;
            }
        }
    }
    return res;
}

vector<const Surface*> Mesh::getMaterialBoundary(
        const uint matId,
        const uint layId) const {
    vector<const Surface*> res;
    vector<const Element*> e = elem_.get(Element::surface, matId, layId);
    res.reserve(e.size());
    for (uint i = 0; i < e.size(); i++) {
        const Surface* surf = dynamic_cast<const Surface*>(e[i]);
        if (surf != NULL) {
            res.push_back(surf);
        }
    }
    return res;
}

vector<BoxD3> Mesh::discretizeWithinBoundary(
        const uint matId,
        const uint layId) const {
#warning "Not implemented"
}


void
Mesh::setMaterialIds(
 const vector<ElementId>& id,
 const MatId newMatId) {
	const uint nIds = id.size();
	for (uint i = 0; i < nIds; i++) {
	    elem_.getPtrToId(id[i])->setMatId(newMatId);
	}
}

vector<uint>
Mesh::getIdsOfCurvedTets() const {
    vector<uint> res;
    uint nK = elem_.nVolumeElements();
    for (uint k = 0; k < nK; k++) {
        if (elem_.tet[k]->isCurved()) {
            res.push_back(elem_.tet[k]->getId());
        }
    }
    return res;
}


vector<Tri3>
Mesh::getTriWithMatId(
        const uint matId,
        const bool ignoreTet) const {
    vector<Tri3> res;
    const uint nTri = elem_.tri.size();
    const uint nTet = elem_.tet.size();
    res.reserve(nTri + nTet);
    // --- Runs over surfaces -------------------------------------------------
    const uint nTri3 = elem_.tri3.size();
    for (uint i = 0; i < nTri3; i++) {
        if (elem_.tri3[i].getMatId() == matId) {
            res.push_back(elem_.tri3[i]);
        }
    }
    const uint nTri6 = elem_.tri6.size();
    for (uint i = 0; i < nTri6; i++) {
        if (elem_.tri6[i].getMatId() == matId) {
            res.push_back(elem_.tri6[i].linearize());
        }
    }
    // --- Runs over tetrahedrons ---------------------------------------------
    if (!ignoreTet) {
        vector<uint> tetIds;
        tetIds.reserve(nTet);
        for (uint i = 0; i < nTet; i++) {
            // Generates list of tetrahedrons ids.
            if (elem_.tet[i]->getMatId() == matId) {
                tetIds.push_back(elem_.tet[i]->getId());
            }
        }
        // Gets internal border of tetrahedron volume.
        vector<pair<const Volume*, uint> > internalBorder;
        internalBorder = getInternalBorder(tetIds);
        // Converts internal border to Tri3.
        for (uint i = 0; i < internalBorder.size(); i++) {
            const Volume* vol = internalBorder[i].first;
            const Tet* tet = dynamic_cast<const Tet*>(vol);
            const uint face = internalBorder[i].second;
            res.push_back(tet->getTri3Face(face));
        }
    }
    return res;
}


vector<Tri3>
Mesh::getTriWithId(const vector<uint>& ids) const {
    vector<Tri3> res;
    res.reserve(elem_.tri3.size());
    for (uint i = 0; i < ids.size(); i++) {
        for (uint j = 0; j < elem_.tri3.size(); j++) {
            if (ids[i] == elem_.tri3[j].getId()) {
                res.push_back(elem_.tri3[j]);
            }
        }
    }
    return res;
}

vector<uint>
Mesh::getTetIds(
 const vector<uint> elemIds) const {
    vector<uint> res;
    res.reserve(elemIds.size());
    const uint nId = elemIds.size();
    for (uint i = 0; i < nId; i++) {
        if (elem_.getPtrToId(elemIds[i])->is<Tet>()) {
            res.push_back(elemIds[i]);
        }
    }
    return res;
}

vector<pair<const Volume*, uint> >
Mesh::getInternalBorder(
 const vector<uint>& region) const {
    // Runs over all elements contained in the region vector detecting
    // the internal border. Returns a vector containing the element
    // faces composing the internal border.
    if (elem_.areTetrahedrons(region)) {
        return getInternalBorderOfTetRegion(region);
    } else if (elem_.areTriangles(region)) {
        return getInternalBorderOfTriRegion(region);
    }
    cerr << endl << "ERROR @ getInternalBorder: "
      << "Unable to detect region." << endl;
}

vector<pair<const Volume*, uint> >
Mesh::getExternalBorder(
 const vector<uint>& elemIds) const {
    // Generates a vector of pairs pointers to tetrahedrons and faces that
    // connects with the region specified by the elemIds inputted.
    // If the element's face in the inputted region is in the computational
    // border nothing is returned.
    vector<pair<const Volume*, uint> > internal;
    internal = getInternalBorder(elemIds);
    uint nI = internal.size();
    vector<pair<const Volume*, uint> > external;
    external.reserve(nI);
    for (uint i = 0; i < nI; i++) {
        uint inId = internal[i].first->getId();
        uint inFace = internal[i].second;
        const Tet* outVol = map_.getNeighbour(inId, inFace);
        uint outFace = map_.getVolToF(inId, inFace);
        if (outVol->getId() != inId || inFace != outFace)  {
            pair<const Volume*, uint> aux(outVol, outFace);
            external.push_back(aux);
        }
    }
    return external;
}


vector<pair<const Volume*, uint> >
Mesh::getInternalBorderOfTetRegion(
 const vector<uint>& region) const {
    // Builds a list with all tetrahedron faces.
    static const uint faces = 4;
    static const uint nVert = 3;
    uint nK = region.size();
    uint nList = nK * faces;
    DynMatrix<uint> fList(nList, 2 + nVert);
    for (uint k = 0; k < nK; k++) {
        const Tet* tet = elem_.getTetPtrToId(region[k]);
        for (uint f = 0; f < faces; f++) {
            uint row = k * faces + f;
            fList(row, 0) = tet->getId();
            fList(row, 1) = f;
            uint ordered[nVert];
            tet->getOrderedSideVerticesId(ordered, f);
            for (uint i = 0; i < nVert; i++) {
                fList(row, i + 2) = ordered[i];
            }
        }
    }
    // Sorts according to the coordinates id. Pairing matching faces. ---------
    // If there is not a pair, then that element is not connected with other
    // remaining connected with itself.
    fList.sortRows_omp(2,4);
    // Copies non repeated faces into result vector.
    vector<pair<const Volume*, uint> > res;
    res.reserve(nK);
    bool matches;
    for (uint k = 0; k < nList; k++) {
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
            const Tet* tet = elem_.getTetPtrToId(fList(k,0));
            uint face = fList(k,1);
            pair<const Tet*, uint> aux(tet, face);
            res.push_back(aux);
        }
    }
    return res;
}

vector<pair<const Volume*, uint> >
Mesh::getInternalBorderOfTriRegion(
 const vector<uint>& region) const {
    uint nE = region.size();
    vector<pair<const Volume*, uint> > res(nE);
    for (uint i = 0; i < nE; i++) {
       const Surface* surf =
        dynamic_cast<const Surface*>(elem_.getPtrToId(region[i]));
        res[i] = getBoundary(surf);
    }
    return res;
}

bool
Mesh::isLinear() const {
	return elem_.isLinear();
}

void
Mesh::linearize() {
	elem_.linearize();
}

BoxD3
Mesh::getBound(const vector<ElementId>& list) const {
	// Inits bounding box.
	if (list.size() == 0) {
	   return BoxD3().setInfinity();
	}
	// Runs over border computing the bounding box of each face.
	BoxD3 bound = elem_.getPtrToId(list[0])->getBound();
	const uint nK = list.size();
	for (uint i = 1; i < nK; i++) {
		const Element* e = elem_.getPtrToId(list[i]);
		bound << e->getBound();
	}
	return bound;
}

BoxD3
Mesh::getBound(
 const vector<pair<const Volume*, uint> >& border) const {
	// Inits bounding box.
   if (border.size() == 0) {
      return BoxD3().setInfinity();
   }
	const Volume* tet = border[0].first;
	const uint face = border[0].second;
	BoxD3 bound = tet->getBoundOfFace(face);
	// Runs over border computing the bounding box of each face.
	for (uint i = 1; i < border.size(); i++) {
		const Volume* vol = border[i].first;
		const uint face = border[i].second;
		bound << vol->getBoundOfFace(face);
	}
	return bound;
}

vector<uint>
Mesh::getIdsInsideBound(
 const BoxD3& bound, const Element::Type type) const {
	const uint nK = elem_.element.size();
	vector<uint> res;
	res.reserve(nK);
	BoxD3 localBound;
	for (uint i = 0; i < nK; i++) {
		const Element* e = elem_.element[i];
		localBound = e->getBound();
		if (localBound <= bound
		 && (e->getType() == type || type==Element::undefined)) {
			res.push_back(e->getId());
		}
	}
	return res;
}

bool
Mesh::isRectilinear() const {
	bool hasCartesianGridDefined = (grid_ != NULL);
	bool onlyContainsQuad4 = (elem_.quad4.size() == elem_.element.size());
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
 const double factor) {
	cG_.applyScalingFactor(factor);
	if (grid_ != NULL) {
		grid_->applyScalingFactor(factor);
	}
}

vector<pair<const Element*, uint> >
Mesh::getElementsWithVertex(
      const uint vertexId,
      const Element::Type type) const {
   vector<pair<const Element*, uint> > res;
   for (uint i = 0; i < elem_.element.size(); i++) {
      const Element* e = elem_.element[i];
      for (uint j = 0; j < e->numberOfVertices(); j++) {
         if (e->getType() == type && e->getVertex(j)->getId() == vertexId) {
            pair<const Element*, uint> aux(e,j);
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

vector<ElementId> Mesh::addAsHex8(const BoxD3& box) {
    cG_.add(box.getPos());
    vector<Hex8> hexes;
    hexes.push_back(Hex8(cG_, ElementId(0), box.getMin(), box.getMax()));
    return elem_.add(cG_, hexes);
}
