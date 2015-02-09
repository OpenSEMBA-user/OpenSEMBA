/*
 * Polygon.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#include "Polygon.h"

Polygon::Polygon() {
}

Polygon::Polygon(
 const CoordinateGroup& cG,
 const uint idIn,
 const vector<uint>& vId,
 const uint matIdNew) {
	assert(vId.size() >= 3);
	id = idIn;
	matId = matIdNew;
	v_.resize(vId.size());
	for (uint i = 0; i < vId.size(); i++) {
		v_[i] = cG.getPtrToId(vId[i]);
	}
}

Polygon::~Polygon() {
}

unsigned int
Polygon::numberOfCoordinates() const {
	return v_.size();
}

unsigned int
Polygon::numberOfVertices() const {
	return numberOfCoordinates();
}

unsigned int
Polygon::numberOfFaces() const {
	return v_.size();
}

unsigned int
Polygon::numberOfSideVertices(const uint f = 0) const {
	return 2;
}

unsigned int
Polygon::numberOfSideCoordinates(const uint f = 0) const {
	return 2;
}

void
Polygon::setV(
 const unsigned int i,
 const Coordinate<double, 3>* constCoordinate) {
	assert(i < numberOfCoordinates());
	v_[i] = constCoordinate;
}

const Coordinate<double, 3>*
Polygon::getV(const unsigned int i) const {
	assert(i < numberOfCoordinates());
	return v_[i];
}

const Coordinate<double, 3>*
Polygon::getSideV(
 const unsigned int f,
 const unsigned int i) const {
	return v_[(f + i) % numberOfCoordinates()];
}

const Coordinate<double, 3>*
Polygon::getVertex(const unsigned int i) const {
	return getV(i);
}

const Coordinate<double, 3>*
Polygon::getSideVertex(
 const unsigned int f,
 const unsigned int i) const {
	return getSideV(f,i);
}

bool
Polygon::isCurved() const {
	return false;
}

void
Polygon::printInfo() const {
	cout<< "--- Polygon info ---" << endl
		<< "Number of coordinates: " << numberOfCoordinates() << endl;
	cout<< "Id: " << getId() << ", MatId: " << getMatId() << endl;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		cout<< "#" << i << ": ";
		v_[i]->printInfo();
		cout<< endl;
	}
	cout<< "--- End of polygon info ---" << endl;
}

double
Polygon::getArea() const {
	cerr<< "ERROR @ Polygon getArea: "
		<< "Not implemented" << endl;
	exit(ELEMENT_ERROR);
}
