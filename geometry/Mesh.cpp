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

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}

void
Mesh::addCoordinates(const Grid3& grid) {
    v.add(grid.getPos());
}

void
Mesh::setMaterialIds(
 const vector<unsigned int>& id,
 const unsigned int newMatId) {
	const unsigned int nIds = id.size();
	for (unsigned int i = 0; i < nIds; i++) {
		getElementWithId(id[i])->setMatId(newMatId);
	}
}

vector<unsigned int>
Mesh::getIdsWithMaterialId(
 const unsigned int matId) const {
	const unsigned int nK = elem.element.size();
	vector<unsigned int> res;
	res.reserve(nK);
	for (unsigned int i = 0; i < nK; i++) {
		if (elem.element[i]->getMatId() == matId) {
			res.push_back(elem.element[i]->getId());
		}
	}
	return res;
}

vector<unsigned int>
Mesh::getIdsWithoutMaterialId(
 const unsigned int matId) const {
	const unsigned int nK = elem.element.size();
	vector<unsigned int> res;
	res.reserve(nK);
	for (unsigned int i = 0; i < nK; i++) {
		if (elem.element[i]->getMatId() != matId) {
			res.push_back(elem.element[i]->getId());
		}
	}
	return res;
}

bool
Mesh::isLinear() const {
	return (elem.isLinear());
}

void
Mesh::linearize() {
	elem.linearize();
	map.build(v,elem);
}

BoxD3
Mesh::getBound(
 const vector<unsigned int>& list) const {
	// Inits bounding box.
	if (list.size() == 0) {
	   return BoxD3().setInfinity();
	}
	// Runs over border computing the bounding box of each face.
	BoxD3 bound = getElementWithId(list[0])->getBound();
	const unsigned int nK = list.size();
	for (unsigned int i = 1; i < nK; i++) {
		const Element* e = getElementWithId(list[i]);
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
   // Runs over border computing the bounding box of each face.
	BoxD3 bound;
	for (unsigned int i = 0; i < border.size(); i++) {
		const Volume* vol = border[i].first;
		const unsigned int face = border[i].second;
		bound << vol->getBoundOfFace(face);
	}
	return bound;
}

vector<unsigned int>
Mesh::getIdsInsideBound(
 const BoxD3& bound, const Element::Type type) const {
	const unsigned int nK = elem.element.size();
	vector<unsigned int> res;
	res.reserve(nK);
	BoxD3 localBound;
	for (unsigned int i = 0; i < nK; i++) {
	   const Element* e = elem.element[i];
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
	bool onlyContainsQuad4 = (elem.quad4.size() == elem.element.size());
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
Mesh::removeHexahedrons() {
	elem.removeHex();
}

bool
Mesh::canExtractGrid() const {
	if (elem.hex8.size() != 0) {
		return true;
	}
	if (grid_ != NULL) {
		return true;
	}
	return false;
}

void
Mesh::applyGeometricScalingFactor(
 const double factor) {
	v.applyScalingFactor(factor);
	if (grid_ != NULL) {
		grid_->applyScalingFactor(factor);
	}
}

Element*
Mesh::getElementWithId(const unsigned int id) {
	return elem.getPtrToId(id);
}

const Element*
Mesh::getElementWithId(const unsigned int id) const {
	return elem.getPtrToId(id);
}
