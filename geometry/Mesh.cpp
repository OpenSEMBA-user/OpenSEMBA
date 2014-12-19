/*
 * Mesh.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */
#include "Mesh.h"

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
	map.clear();
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
 const vector<pair<const Tet*, unsigned int> >& border) const {
	// Inits bounding box.
   if (border.size() == 0) {
      return BoxD3().setInfinity();
   }
	const Tet* tet = border[0].first;
	const unsigned int face = border[0].second;
	BoxD3 bound = tet->getBoundOfFace(face);
	// Runs over border computing the bounding box of each face.
	const unsigned int nK = border.size();
	for (unsigned int i = 1; i < nK; i++) {
		const Tet* tet = border[i].first;
		const unsigned int face = border[i].second;
		bound << tet->getBoundOfFace(face);
	}
	return bound;
}

vector<unsigned int>
Mesh::getIdsInsideBound(
 const pair<CVecD3,CVecD3>& bound) const {
	const unsigned int nK = elem.element.size();
	vector<unsigned int> res;
	res.reserve(nK);
	BoxD3 localBound;
	for (unsigned int i = 0; i < nK; i++) {
		const Element* e = elem.element[i];
		localBound = e->getBound();
		bool isInside = true;
		for (unsigned int j = 0; j < 3; j++) {
			isInside &=
			 (localBound.getMin()(j) >= bound.first(j)
			 && localBound.getMax()(j) <= bound.second(j));
		}
		if (isInside) {
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

Grid3
Mesh::getGrid() const {
	if (!canExtractGrid()) {
		exit(-1);
	}
	if (grid_ != NULL) {
		return *grid_;
	}
	Grid3 res = getGridFromHexahedrons();
	return res;
}

Grid3
Mesh::getGridFromHexahedrons() const {
	// Computes global bound.
	vector<unsigned int> ids = elem.getHexIds();
	BoxD3 bound = getBound(ids);
	// Computes cell size.
	assert(elem.hex8.size() != 0);
	BoxD3 hexBound = elem.hex8[0].getBound();
	CVecD3 cellSize = hexBound.getMax() - hexBound.getMin();
	bool isCartesian = elem.hex8[0].isRegular() ;
//	for (unsigned int i = 1; i < elem.hex8.size(); i++) {
//		isCartesian &= elem.hex8[i].isRegular();
//		elem.hex8[i].printInfo();
//		hexBound = elem.hex8[i].getBound();
//		CVecD3 auxCS = hexBound.second - hexBound.first;
//		isCartesian &= (auxCS == cellSize);
//	}
	if (isCartesian) {
		CVecD3 cellSize = hexBound.getMax() - hexBound.getMin();
		return Grid3(bound, cellSize);
	} else {
		cerr << "ERROR @ Getting grid from mesh." << endl;
		cerr << "Hexahedrons do not form a cartesian grid." << endl;
		exit(MESH_ERROR);
	}
	cerr << "ERROR @ Mesh" << endl;
	exit(MESH_ERROR);
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
	cerr << "ERROR @ Getting grid from mesh." << endl;
	cerr << "Mesh does not contain hexahedrons." << endl;
	cerr << "No grid information is stored." << endl;
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
