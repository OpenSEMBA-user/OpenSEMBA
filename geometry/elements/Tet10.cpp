/*
 * Tet10.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET10_H_
#include "Tet10.h"
#endif

Tet10::Tet10(
 const CoordinateGroup& coordGr,
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int vId[10]) {
	id = id_;
	matId = matId_;
	for (unsigned int i = 0; i < tet.np; i++) {
		v[i] = coordGr.getPtrToId(vId[i]);
	}
	//
	check();
}

Tet10::~Tet10() {

}

Tet10&
Tet10::operator=(const Tet10& rhs) {
	if (this == &rhs)
		return *this;
	id = rhs.id;
	matId = rhs.matId;
	for (unsigned int i = 0; i < 10; i++) {
		v[i] = rhs.v[i];
	}
	return *this;
}

void
Tet10::setV(const unsigned int i, const Coordinate<double,3>* vNew) {
	v[i] = vNew;
}

const Coordinate<double,3>*
Tet10::getSideV(const unsigned int f, const unsigned int i) const {
	return v[tet.sideNode(f,i)];
}

const Coordinate<double,3>*
Tet10::getVertex(const unsigned int i) const {
	return v[tet.vertex(i)];
}

const Coordinate<double,3>*
Tet10::getSideVertex(const unsigned int f, const unsigned int i) const {
	return v[tet.sideVertex(f, i)];
}

bool
Tet10::isCurved() const {
	for (unsigned int f = 0; f < tet.faces; f++) {
		if(isCurvedFace(f)) {
			return true;
		}
	}
	return false;
}

bool
Tet10::isCurvedFace(const unsigned int f) const {
	return getTri6Face(f).isCurved();
}

double
Tet10::getVolume() const {
	double cJDet[SimplexTet<2>::ncp];
	getCubatureJacobianDeterminant(cJDet);
	double res = 0.0;
	for (register unsigned int c = 0; c < SimplexTet<2>::ncp; c++) {
		res += tet.cw[c] * cJDet[c];
	}
	res *= double(1.0 / 6.0);
	return res;
}

double
Tet10::getAreaOfFace(const unsigned int face) const {
	return getTri6Face(face).getArea();
}

void
Tet10::printInfo() const {
	cout << "--- Tet10 info ---" << endl;
	cout << "Id: " << id << endl;
	cout << "Coordinates:" << endl;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}

Tri6
Tet10::getTri6Face(const unsigned int f) const {
	const Coordinate<double,3>* sideV[6];
	for (unsigned int i = 0; i < 6; i++) {
		sideV[i] = getSideV(f,i);
	}
	Tri6 auxFace(sideV);
	return auxFace;
}

Tet4
Tet10::linearize() const {
	const Coordinate<double,3>* vertex[4];
	for (unsigned int i = 0; i < 4; i++) {
		vertex[i] = getVertex(i);
	}
	Tet4 res(vertex, id, matId, layerId);
	return res;
}

bool
Tet10::isFaceContainedInPlane(
 const unsigned int face,
 const CartesianPlane plane) const {
	return getTri6Face(face).isContainedInPlane(plane);
}

void
Tet10::check() const {

}
