/*
 * Polyhedron.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: luis
 */

#include "Polyhedron.h"

Polyhedron::Polyhedron() {
}

Polyhedron::~Polyhedron() {
}

Polyhedron::Polyhedron(
 const uint idNew,
 const vector<const Polygon*>& face) {
	face_ = face;
	id = idNew;
	for (uint f = 0; f < numberOfFaces(); f++) {
		for (uint i = 0; i < face[f]->numberOfCoordinates(); i++) {
			addV(face_[f]->getV(i));
		}
	}
	checkClosedness();
}

const Polygon*
Polyhedron::getFace(const uint f) const {
	assert(f < numberOfFaces());
	return face_[f];
}

unsigned int
Polyhedron::numberOfCoordinates() const {
	return v_.size();
}

unsigned int
Polyhedron::numberOfVertices() const {
	return numberOfCoordinates();
}

unsigned int
Polyhedron::numberOfFaces() const {
	return face_.size();
}

unsigned int
Polyhedron::numberOfSideVertices(const uint f) const {
	assert(f < numberOfFaces());
	return face_[f]->numberOfVertices();
}

unsigned int
Polyhedron::numberOfSideCoordinates(const uint f) const {
	return numberOfSideVertices(f);
}

const Coordinate<double, 3>*
Polyhedron::getV(
 const unsigned int i) const {
	assert(i < numberOfCoordinates());
	return v_[i];
}

const Coordinate<double, 3>*
Polyhedron::getSideV(
 const unsigned int f,
 const unsigned int i) const {
	return getFace(f)->getV(i);
}

const Coordinate<double, 3>*
Polyhedron::getVertex(const unsigned int i) const {
	return getV(i);
}

const Coordinate<double, 3>*
Polyhedron::getSideVertex(
 const unsigned int f,
 const unsigned int i) const {
	return getSideV(f,i);
}

bool
Polyhedron::isCurved() const {
	return false;
}

bool
Polyhedron::isCurvedFace(const unsigned int face) const {
	return getFace(face)->isCurved();
}

double
Polyhedron::getAreaOfFace(const unsigned int face) const {
	return getFace(face)->getArea();
}

void
Polyhedron::printInfo() const {
	cout<< "--- Polyhedron info ---" << endl
		<< "Number of vertices: " << numberOfVertices() << endl
		<< "Number of faces: " << numberOfFaces() << endl;
	for (uint i = 0; i < numberOfFaces(); i++) {
		cout<< "Face #" << i << endl;
		getFace(i)->printInfo();
	}
}

void
Polyhedron::addV(const Coordinate<double, 3>* v) {
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		if (*v_[i] == *v) {
			return;
		}
	}
	v_.push_back(v);
}

void
Polyhedron::checkClosedness() const {
	// This checks consists in pairing segments ordered according to their ids.
	// If a segment is not paired, the surface is not closed.
	uint nSidesInFaces = 0;
	for (uint f = 0; f < numberOfFaces(); f++) {
		nSidesInFaces += getFace(f)->numberOfFaces();
	}
	DynMatrix<uint> list(nSidesInFaces, 2);
	uint row = 0;
	for (uint f = 0; f < numberOfFaces(); f++) {
		for (uint s = 0; s < getFace(f)->numberOfFaces(); s++) {
			uint id0 = getFace(f)->getSideVertex(s,0)->getId();
			uint id1 = getFace(f)->getSideVertex(s,1)->getId();
			if (id0 < id1) {
				list(row,0) = id0;
				list(row,1) = id1;
			} else {
				list(row,0) = id1;
				list(row,1) = id0;
			}
		    row++;
		}
	}
	list.sortRows(0,1);
	for (uint i = 0; i < nSidesInFaces; i += 2) {
		if (list(i,0) != list(i+1,0) || list(i,1) != list(i+1,1)) {
			cerr<< "ERROR @ Polyhedron: "
				<< "Polyhedron is not closed." << endl;
			printInfo();
		}
	}
}
