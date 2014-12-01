/*
 * Quad4.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#include "Quad4.h"

Quad4::Quad4() {
}

Quad4::~Quad4() {
}

Quad4::Quad4(
 const CoordinateGroup& cG,
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int vId[4]) {
	id = id_;
	matId = matId_;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		v[i] = cG.getPtrToId(vId[i]);
	}
	// TODO Normals are not handled.
	check();
}

Quad4&
Quad4::operator =(
 const Quad4& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id = rhs.id;
	matId = rhs.matId;
	normal = rhs.normal;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i] = rhs.v[i];
	}
	return *this;
}

void
Quad4::setV(
 const unsigned int i,
 const Coordinate<double, 3>* constCoordinate) {
	v[i] = constCoordinate;
}

const Coordinate<double, 3>*
Quad4::getVertex(const unsigned int i) const {
	return v[i];
}

void
Quad4::printInfo() const {
	cout << "--- Quad4 info ---" << endl;
	cout << "Id: " << id << "  Mat Id: " << matId << endl;
	cout << "Normal: ";
	normal.printInfo();
	cout << endl;
	cout << "Coordinates:" << endl;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}

void
Quad4::check() const {
	// TODO Auto-generated
}

const Coordinate<double, 3>*
Quad4::getSideV(
 const unsigned int face,
 const unsigned int i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v[(face + i) % 4];
}

const Coordinate<double, 3>*
Quad4::getSideVertex(
 const unsigned int face,
 const unsigned int i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v[(face + i) % 4];
}
