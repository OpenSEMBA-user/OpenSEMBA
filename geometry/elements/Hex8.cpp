/*
 * Hex8.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#include "Hex8.h"

Hex8::Hex8() {
	// TODO Auto-generated constructor stub

}

Hex8::Hex8(
 const CoordinateGroup<>& coordGr,
 const unsigned int id_,
 const unsigned int matId_,
 const CoordinateId vId[8]) {
	id = id_;
	matId = matId_;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Hex8::Hex8(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->isOf<CoordD3>()) {
            cerr << "ERROR @ Hex8::Hex8(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v[i] = coord->castTo<CoordD3>();
	}
}

Hex8::Hex8(
 const CoordinateGroup<>& cG,
 const CVecD3& min,
 const CVecD3& max,
 const uint matId_) {
	id = 0;
	matId = matId_;
	v[0] = cG.get(CVecD3(min(0), min(1), min(2)));
	v[1] = cG.get(CVecD3(min(0), min(1), max(2)));
	v[2] = cG.get(CVecD3(min(0), max(1), min(2)));
	v[3] = cG.get(CVecD3(min(0), max(1), max(2)));
	v[4] = cG.get(CVecD3(max(0), min(1), min(2)));
	v[5] = cG.get(CVecD3(max(0), min(1), max(2)));
	v[6] = cG.get(CVecD3(max(0), max(1), min(2)));
	v[7] = cG.get(CVecD3(max(0), max(1), max(2)));
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		if (v[i] == NULL) {
			cerr<< "ERROR @ Hex8 ctor: "
				<< "Coordinate " << i << " pointer is null." << endl;
            assert(false);
            exit(ELEMENT_ERROR);
		}
	}
}
Hex8::~Hex8() {
	// TODO Auto-generated destructor stub
}

double
Hex8::getAreaOfFace(const unsigned int f) const {
	CartesianVector<double,3> v1, v2;
	v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
	v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
	return ((double) 0.5 * (v1 ^ v2).norm());
}

const Coordinate<double, 3>*
Hex8::getSideV(const unsigned int face, const unsigned int i) const {
	assert(face < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	switch (face) {
	case 0:
		switch (i) {
		case 0:
			return v[0];
			break;
		case 1:
			return v[1];
			break;
		case 2:
			return v[2];
			break;
		case 3:
			return v[3];
			break;
		}
		break;
	case 1:
		switch (i) {
		case 0:
			return v[0];
			break;
		case 1:
			return v[3];
			break;
		case 2:
			return v[7];
			break;
		case 3:
			return v[4];
			break;
		}
		break;
	case 2:
		switch (i) {
		case 0:
			return v[0];
			break;
		case 1:
			return v[4];
			break;
		case 2:
			return v[5];
			break;
		case 3:
			return v[1];
			break;
		}
		break;
	case 3:
		switch (i) {
		case 0:
			return v[1];
			break;
		case 1:
			return v[5];
			break;
		case 2:
			return v[6];
			break;
		case 3:
			return v[2];
			break;
		}
		break;
	case 4:
		switch (i) {
		case 0:
			return v[2];
			break;
		case 1:
			return v[6];
			break;
		case 2:
			return v[7];
			break;
		case 3:
			return v[3];
			break;
		}
		break;
	case 5:
		switch (i) {
		case 0:
			return v[4];
			break;
		case 1:
			return v[7];
			break;
		case 2:
			return v[6];
			break;
		case 3:
			return v[5];
			break;
		}
		break;
	}
	cerr << "ERROR @ Hex8" << endl;
	cerr << "Side vertex not found." << endl;
	exit(-1);
}

const Coordinate<double, 3>*
Hex8::getSideVertex(
 const unsigned int face,
 const unsigned int i) const {
	return getSideV(face,i);
}

void
Hex8::setV(
 const unsigned int i,
 const Coordinate<double, 3>* constCoordinate) {
	assert(i < numberOfCoordinates());
	v[i] = constCoordinate;
}

bool
Hex8::isRegular() const {
	// Checks that all edges are aligned with one of the axis.
	static const CVecD3 xAxe(1.0, 0.0, 0.0);
	static const CVecD3 yAxe(0.0, 1.0, 0.0);
	static const CVecD3 zAxe(1.0, 0.0, 1.0);
	for (unsigned int f = 0; f < numberOfFaces(); f++) {
		Coordinate<double,3> first, second;
		CVecD3 inc;
		for (unsigned int i = 0; i < numberOfSideVertices(); i++) {
			first = *getSideV(f, i);
			second = *getSideV(f, (i+1) % numberOfSideVertices());
			inc = (second - first).normalize();
			if ( !( (abs(inc.dot(xAxe))-1.0) <= _Hex8_nspace::tolerance
			 || (abs(inc.dot(yAxe))-1.0) <= _Hex8_nspace::tolerance
			 || (abs(inc.dot(zAxe))-1.0) <= _Hex8_nspace::tolerance) ) {
				return false;
			}
		}
	}
	return true;
}

void
Hex8::printInfo() const {
	cout << "--- Hex8 Info ---" << endl;
	cout << "Id: " << getId() << endl;
	cout << "Coordinates:" << endl;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		getV(i)->printInfo();
		cout << endl;
	}
	if (isRegular()) {
		cout << "Is regular Hex." << endl;
	} else {
		cout << "Is not a regular Hex." << endl;
	}
}

