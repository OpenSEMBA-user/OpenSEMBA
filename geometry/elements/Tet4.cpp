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
 const CoordinateGroup<>& coordGr,
 const CoordinateId vId[4],
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int layerId_) :
         Tet(id_, matId_, layerId_) {
	for (unsigned int i = 0; i < tet.np; i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Tet4::Tet4(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->isOf<CoordD3>()) {
            cerr << "ERROR @ Tet4::Tet4(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v[i] = coord->castTo<CoordD3>();
	}
	check();
}

Tet4::Tet4(
 const CoordD3* v_[4],
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int layerId_) :
        Tet(id_, matId_, layerId_) {
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
	Tet::operator=(rhs);
	for (unsigned int i = 0; i < 4; i++) {
		v[i] = rhs.v[i];
	}
	return *this;
}

double
Tet4::getVolume() const {
	StaMatrix<double,3,3> mat;
	CVecD3 aux;
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
	CVecD3 v1, v2;
	v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
	v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
	return ((double) 0.5 * (v1 ^ v2).norm());
}

void
Tet4::setV(
 const unsigned int i,
 const CoordD3* vNew) {
	v[i] = vNew;
}

const CoordD3*
Tet4::getSideV(const unsigned int f, const unsigned int i) const {
	return v[tet.sideNode(f,i)];
}

bool
Tet4::isCurved() const {
	return false;
}

const CoordD3*
Tet4::getSideVertex(const unsigned int f, const unsigned int i) const {
	return v[tet.sideVertex(f,i)];
}

void
Tet4::check() const {
	if(hasZeroVolume()) {
		cerr << "ERROR @ Tet4::check():"
		     << "Element " << id << " has null volume." << endl;
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

bool
Tet4::hasZeroVolume() const {
	bool zeroVolume;
	CVecD3 initialVCoord, otherVCoord;
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

bool
Tet4::isInnerPoint(const CVecD3& pos) const {
    if (!getBound().isInnerPoint(pos)) {
        return false;
    }
    // Checks if point is inside a tetrahedron using the following algorithm:
    // http://steve.hollasch.net/cgindex/geometry/ptintet.html
    StaMatrix<double,4,4> mat;
    // Builds matrix D0.
    for (uint i = 0; i < 4; i++) {
        for (uint j = 0; j < 3; j++) {
            mat(i,j) = getVertex(i)->pos()(j);
        }
        mat(i,3) = (double) 1.0;
    }
    double det = mat.getDeterminant4x4();
    assert(det != 0);
    bool isPositive = (det > (double) 0.0);
    // Checks rest of matrices. Last column is always 1.0.
    for (uint k = 0; k < 4; k++) {
        // Copies pos in row k.
        for (uint j = 0; j < 3; j++) {
            mat(k,j) = pos(j);
        }
        // Copies rest of vertices.
        for (uint i = 0; i < 4; i++) {
            if (i != k) {
                for (uint j = 0; j < 3; j++) {
                    mat(i,j) = getVertex(i)->pos()(j);
                }
            }
        }
        double det = mat.getDeterminant4x4();
        if ((det > (double) 0.0) != isPositive) {
            return false;
        }
    }
    return true;
}
