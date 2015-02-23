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
        const CoordinateGroup& cG,
        const Grid3* grid) {
    cG_ = cG;
    if (grid != NULL) {
        grid_ = new Grid3(*grid);
    } else {
        grid_ = NULL;
    }
}

Mesh::Mesh(
        const CoordinateGroup& cG,
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

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}

void
Mesh::setMaterialIds(
 const vector<unsigned int>& id,
 const unsigned int newMatId) {
	const unsigned int nIds = id.size();
	for (unsigned int i = 0; i < nIds; i++) {
	    elem_.getPtrToId(id[i])->setMatId(newMatId);
	}
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
Mesh::getBound(
 const vector<unsigned int>& list) const {
	// Inits bounding box.
	if (list.size() == 0) {
	   return BoxD3().setInfinity();
	}
	// Runs over border computing the bounding box of each face.
	BoxD3 bound = elem_.getPtrToId(list[0])->getBound();
	const unsigned int nK = list.size();
	for (unsigned int i = 1; i < nK; i++) {
		const Element* e = elem_.getPtrToId(list[i]);
		bound << e->getBound();
	}
	return bound;
}

BoxD3
Mesh::getBound(
 const vector<pair<const Volume*, unsigned int> >& border) const {
	// Inits bounding box.
   if (border.size() == 0) {
      return BoxD3().setInfinity();
   }
	const Volume* tet = border[0].first;
	const unsigned int face = border[0].second;
	BoxD3 bound = tet->getBoundOfFace(face);
	// Runs over border computing the bounding box of each face.
	for (unsigned int i = 1; i < border.size(); i++) {
		const Volume* vol = border[i].first;
		const unsigned int face = border[i].second;
		bound << vol->getBoundOfFace(face);
	}
	return bound;
}

vector<unsigned int>
Mesh::getIdsInsideBound(
 const BoxD3& bound, const Element::Type type) const {
	const unsigned int nK = elem_.element.size();
	vector<unsigned int> res;
	res.reserve(nK);
	BoxD3 localBound;
	for (unsigned int i = 0; i < nK; i++) {
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
}
