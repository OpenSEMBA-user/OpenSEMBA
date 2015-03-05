/*
 * Tet10.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET10_H_
#include "Tet10.h"
#endif

Tet10::Tet10() {

}

Tet10::Tet10(const CoordinateGroup<>& coordGr,
             const ElementId id,
             const CoordinateId vId[10],
             const LayerId layerId,
             const uint matId)
:   Tet(id, layerId, matId) {

    for (uint i = 0; i < tet.np; i++) {
        const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Tet10::Tet10(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->isOf<CoordD3>()) {
            cerr << "ERROR @ Tet10::Tet10(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordD3>();
    }
    //
    check();
}

Tet10::Tet10(const Tet10& rhs)
:   Tet(rhs) {

    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tet10::Tet10(const ElementId id, const Tet10& rhs)
:   Tet(id, rhs) {

    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tet10::~Tet10() {

}

ClassBase* Tet10::clone() const {
    return new Tet10(*this);
}

ClassBase* Tet10::clone(const ElementId id) const {
    return new Tet10(id, *this);
}

bool Tet10::isCurved() const {
    for (uint f = 0; f < tet.faces; f++) {
        if(isCurvedFace(f)) {
            return true;
        }
    }
    return false;
}

bool Tet10::isCurvedFace(const uint f) const {
    return getTri6Face(f).isCurved();
}

bool
Tet10::isFaceContainedInPlane(
 const uint face,
 const CartesianPlane plane) const {
    return getTri6Face(face).isContainedInPlane(plane);
}

const CoordD3* Tet10::getSideV(const uint f, const uint i) const {
	return v_[tet.sideNode(f,i)];
}

const CoordD3*
Tet10::getVertex(const uint i) const {
	return v_[tet.vertex(i)];
}

const CoordD3* Tet10::getSideVertex(const uint f, const uint i) const {
	return v_[tet.sideVertex(f, i)];
}

double Tet10::getVolume() const {
	double cJDet[SimplexTet<2>::ncp];
	getCubatureJacobianDeterminant(cJDet);
	double res = 0.0;
	for (register uint c = 0; c < SimplexTet<2>::ncp; c++) {
		res += tet.cw[c] * cJDet[c];
	}
	res *= double(1.0 / 6.0);
	return res;
}

double Tet10::getAreaOfFace(const uint f) const {
	return getTri6Face(f).getArea();
}

Tri6 Tet10::getTri6Face(const uint f) const {
	const CoordD3* sideV[6];
	for (uint i = 0; i < 6; i++) {
		sideV[i] = getSideV(f,i);
	}
	Tri6 auxFace(ElementId(0), sideV);
	return auxFace;
}

void Tet10::setV(const uint i, const CoordD3* vNew) {
    v_[i] = vNew;
}

Tet4 Tet10::linearize() const {
    const Coordinate<double,3>* vertex[4];
    for (uint i = 0; i < 4; i++) {
        vertex[i] = getVertex(i);
    }
    Tet4 res(getId(), vertex, getLayerId(), getMatId());
    return res;
}

void Tet10::printInfo() const {
    cout << "--- Tet10 info ---" << endl;
    cout << "Id: " << getId() << endl;
    cout << "Coordinates:" << endl;
    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
        cout << endl;
    }
}

void Tet10::check() const {

}
