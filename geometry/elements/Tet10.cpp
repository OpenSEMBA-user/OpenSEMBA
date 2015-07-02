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

Tet10::Tet10(const GroupCoordinates<CoordR3>& coordGr,
             const ElementId id,
             const CoordinateId vId[10],
             const LayerId layerId,
             const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    for (UInt i = 0; i < tet.np; i++) {
        v_[i] = coordGr.getId(vId[i]);
    }
    //
    check();
}

Tet10::Tet10(const Tet10& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tet10::~Tet10() {

}

bool Tet10::isCurved() const {
    for (UInt f = 0; f < tet.faces; f++) {
        if(isCurvedFace(f)) {
            return true;
        }
    }
    return false;
}

bool Tet10::isCurvedFace(const UInt f) const {
    return getTri6Face(f).isCurved();
}

bool
Tet10::isFaceContainedInPlane(
 const UInt face,
 const CartesianPlane plane) const {
    return getTri6Face(face).isContainedInPlane(plane);
}

const CoordR3* Tet10::getSideV(const UInt f, const UInt i) const {
	return v_[tet.sideNode(f,i)];
}

const CoordR3*
Tet10::getVertex(const UInt i) const {
	return v_[tet.vertex(i)];
}

const CoordR3* Tet10::getSideVertex(const UInt f, const UInt i) const {
	return v_[tet.sideVertex(f, i)];
}

Real Tet10::getVolume() const {
	Real cJDet[SimplexTet<2>::ncp];
	getCubatureJacobianDeterminant(cJDet);
	Real res = 0.0;
	for (register UInt c = 0; c < SimplexTet<2>::ncp; c++) {
		res += tet.cw[c] * cJDet[c];
	}
	res *= Real(1.0 / 6.0);
	return res;
}

Real Tet10::getAreaOfFace(const UInt f) const {
	return getTri6Face(f).getArea();
}

Tri6 Tet10::getTri6Face(const UInt f) const {
	const CoordR3* sideV[6];
	for (UInt i = 0; i < 6; i++) {
		sideV[i] = getSideV(f,i);
	}
	Tri6 auxFace(ElementId(0), sideV);
	return auxFace;
}

void Tet10::setV(const UInt i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Tet4* Tet10::linearize() const {
    const Coordinate<Real,3>* vertex[4];
    for (UInt i = 0; i < 4; i++) {
        vertex[i] = getVertex(i);
    }
    return new Tet4(getId(), vertex, getLayerId(), getMatId());
}

void Tet10::printInfo() const {
    cout << "--- Tet10 info ---" << endl;
    cout << "Id: " << getId() << endl;
    cout << "Coordinates:" << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

void Tet10::check() const {

}
