/*
 * Tet4.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET4_H_
#include "Tet4.h"
#endif

Tet4::Tet4() {

}

Tet4::Tet4(
 const CoordinateGroup& coordGr,
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int vId[4]) {
	id = id_;
	matId = matId_;
	for (unsigned int i = 0; i < tet.np; i++) {
		v[i] = coordGr.getPtrToId(vId[i]);
	}
	check();
}

Tet4::Tet4(
 const unsigned int id_,
 const unsigned int matId_,
 const Coordinate<double,3>* v_[4]) {
	id = id_;
	matId = matId_;
	for (unsigned int i = 0; i < tet.np; i++) {
		v[i] = v_[i];
	}
	check();
}

Tet4::~Tet4() {

}

Tet4&
Tet4::operator=(const Tet4& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id = rhs.id;
	matId = rhs.matId;
	for (unsigned int i = 0; i < 4; i++) {
		v[i] = rhs.v[i];
	}
	return *this;
}

double
Tet4::getVolume() const {
	StaMatrix<double,3,3> mat;
	CartesianVector<double,3> aux;
	for (unsigned int i = 1; i < 4; i++) {
		aux = getV(0)->pos() - getV(i)->pos();
		for (unsigned int j = 0; j < 3; j++) {
			mat(i-1,j) = aux(j);
		}
	}
	double det = mat.getDeterminant3x3();
	return (det / ((double) 6.0));
}

double
Tet4::getAreaOfFace(const unsigned int f) const {
	CartesianVector<double,3> v1, v2;
	v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
	v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
	return ((double) 0.5 * (v1 ^ v2).norm());
}

void
Tet4::setV(
 const unsigned int i,
 const Coordinate<double,3>* vNew) {
	v[i] = vNew;
}

const Coordinate<double,3>*
Tet4::getSideV(const unsigned int f, const unsigned int i) const {
	return v[tet.sideNode(f,i)];
}

bool
Tet4::isCurved() const {
	return false;
}

const Coordinate<double,3>*
Tet4::getSideVertex(const unsigned int f, const unsigned int i) const {
	return v[tet.sideVertex(f,i)];
}

void
Tet4::check() const {
	if(hasZeroVolume()) {
		cerr << "ERROR@Tet4::check()" << endl;
		cerr << "Element " << id << " has null volume."  << endl;
		cerr << "Terminating"                            << endl;
		exit(ELEMENT_ERROR);
	}
}

void
Tet4::printInfo() const {
	cout << "--- Tet4 info ---" << endl;
	cout << "Id: " << id << endl;
	cout << "Coordinates:" << endl;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}

bool
Tet4::isCurvedFace(const unsigned int face) const {
	return false;
}

bool
Tet4::isFaceContainedInPlane(
 const unsigned int face,
 const CartesianPlane plane) const {
	return getTri3Face(face).isContainedInPlane(plane);
}

Tri3
Tet4::getTri3Face(const unsigned int f) const {
	const Coordinate<double,3>* sideV[3];
	for (unsigned int i = 0; i < 3; i++) {
		sideV[i] = getSideV(f,i);
	}
	Tri3 auxFace(sideV);
	return auxFace;
}

bool
Tet4::hasZeroVolume() const {
	bool zeroVolume;
	CartesianVector<double, 3> initialVCoord, otherVCoord;
	initialVCoord = *v[0];
	for (unsigned int d = 0; d < 3; d++) {
		zeroVolume = true;
		for (unsigned int i = 1; i < tet.np; i++) {
			otherVCoord = *v[i];
			zeroVolume &= (initialVCoord(d) == otherVCoord(d));
		}
		if (zeroVolume) {
			return true;
		}
	}
	return false;
}
