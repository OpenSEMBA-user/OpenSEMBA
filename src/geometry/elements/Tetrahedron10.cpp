// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * Tet10.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET10_H_
#include <geometry/elements/Tetrahedron10.h>
#endif

Tetrahedron10::Tetrahedron10() {

}

Tetrahedron10::Tetrahedron10(const GroupCoordinates<CoordR3>& coordGr,
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

Tetrahedron10::Tetrahedron10(const Tetrahedron10& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Tetrahedron10::~Tetrahedron10() {

}

bool Tetrahedron10::isCurved() const {
    for (UInt f = 0; f < tet.faces; f++) {
        if(isCurvedFace(f)) {
            return true;
        }
    }
    return false;
}

bool Tetrahedron10::isCurvedFace(const UInt f) const {
    return getTri6Face(f).isCurved();
}

bool
Tetrahedron10::isFaceContainedInPlane(
 const UInt face,
 const CartesianPlane plane) const {
    return getTri6Face(face).isContainedInPlane(plane);
}

const CoordR3* Tetrahedron10::getSideV(const UInt f, const UInt i) const {
	return v_[tet.sideNode(f,i)];
}

const CoordR3*
Tetrahedron10::getVertex(const UInt i) const {
	return v_[tet.vertex(i)];
}

const CoordR3* Tetrahedron10::getSideVertex(const UInt f, const UInt i) const {
	return v_[tet.sideVertex(f, i)];
}

Real Tetrahedron10::getVolume() const {
	Real cJDet[SimplexTet<2>::ncp];
	getCubatureJacobianDeterminant(cJDet);
	Real res = 0.0;
	for (register UInt c = 0; c < SimplexTet<2>::ncp; c++) {
		res += tet.cw[c] * cJDet[c];
	}
	res *= Real(1.0 / 6.0);
	return res;
}

Real Tetrahedron10::getAreaOfFace(const UInt f) const {
	return getTri6Face(f).getArea();
}

Triangle6 Tetrahedron10::getTri6Face(const UInt f) const {
	const CoordR3* sideV[6];
	for (UInt i = 0; i < 6; i++) {
		sideV[i] = getSideV(f,i);
	}
	Triangle6 auxFace(ElementId(0), sideV);
	return auxFace;
}

void Tetrahedron10::setV(const UInt i, const CoordR3* vNew) {
    v_[i] = vNew;
}

Tetrahedron4* Tetrahedron10::linearize() const {
    const Coordinate<Real,3>* vertex[4];
    for (UInt i = 0; i < 4; i++) {
        vertex[i] = getVertex(i);
    }
    return new Tetrahedron4(getId(), vertex, getLayerId(), getMatId());
}

void Tetrahedron10::printInfo() const {
    cout << "--- Tet10 info ---" << endl;
    cout << "Id: " << getId() << endl;
    cout << "Coordinates:" << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

void Tetrahedron10::check() const {

}
