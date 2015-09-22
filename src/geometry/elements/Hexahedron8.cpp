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
 * Hexa8.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#include <geometry/elements/Hexahedron8.h>

template<class T>
const Real Hexahedron8<T>::tolerance = 1e-15;

template<class T>
Hexahedron8<T>::Hexahedron8() {

}

template<class T>
Hexahedron8<T>::Hexahedron8(const GroupCoordinates<Coordinate<T,3> >& coordGr,
              const ElementId id,
              const CoordinateId vId[8],
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

	for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = coordGr.getId(vId[i]);
	}
}

template<class T>
Hexahedron8<T>::Hexahedron8(GroupCoordinates<Coordinate<T,3> >& cG,
              const ElementId id,
              const Box<T,3>& box,
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    if(!box.isVolume()) {
        throw typename Box<T,3>::ErrorNotVolume();
    }
    vector<CartesianVector<T,3> > pos = box.getPos();
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
	    v_[i] = cG.getPos(pos[i]);
		if (v_[i] == NULL) {
			v_[i] = cG.addPos(pos[i]);
		}
	}
}

template<class T>
Hexahedron8<T>::Hexahedron8(const Hexahedron8<T>& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
Hexahedron8<T>::~Hexahedron8() {

}

template<class T>
bool Hexahedron8<T>::isStructured(const Grid3& grid, const Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isVolume()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
}

template<class T>
bool Hexahedron8<T>::isRegular() const {
    // Checks that all edges are aligned with one of the axis.
    static const CartesianVector<T,3> xAxe(1.0, 0.0, 0.0);
    static const CartesianVector<T,3> yAxe(0.0, 1.0, 0.0);
    static const CartesianVector<T,3> zAxe(1.0, 0.0, 1.0);
    for (UInt f = 0; f < numberOfFaces(); f++) {
        CartesianVector<T,3> first, second;
        CartesianVector<T,3> inc;
        for (UInt i = 0; i < numberOfSideVertices(); i++) {
            first = *getSideV(f, i);
            second = *getSideV(f, (i+1) % numberOfSideVertices());
            inc = (second - first).normalize();
            if (!((abs(inc.dot(xAxe))-1.0) <= tolerance ||
                  (abs(inc.dot(yAxe))-1.0) <= tolerance ||
                  (abs(inc.dot(zAxe))-1.0) <= tolerance)) {

                return false;
            }
        }
    }
    return true;
}

template<class T>
const Coordinate<T,3>* Hexahedron8<T>::getSideV(const UInt f, const UInt i) const {
    assert(f < numberOfFaces());
    assert(i < numberOfSideCoordinates());
    switch (f) {
    case 0:
        switch (i) {
        case 0:
            return v_[0];
            break;
        case 1:
            return v_[1];
            break;
        case 2:
            return v_[2];
            break;
        case 3:
            return v_[3];
            break;
        }
        break;
    case 1:
        switch (i) {
        case 0:
            return v_[0];
            break;
        case 1:
            return v_[3];
            break;
        case 2:
            return v_[7];
            break;
        case 3:
            return v_[4];
            break;
        }
        break;
    case 2:
        switch (i) {
        case 0:
            return v_[0];
            break;
        case 1:
            return v_[4];
            break;
        case 2:
            return v_[5];
            break;
        case 3:
            return v_[1];
            break;
        }
        break;
    case 3:
        switch (i) {
        case 0:
            return v_[1];
            break;
        case 1:
            return v_[5];
            break;
        case 2:
            return v_[6];
            break;
        case 3:
            return v_[2];
            break;
        }
        break;
    case 4:
        switch (i) {
        case 0:
            return v_[2];
            break;
        case 1:
            return v_[6];
            break;
        case 2:
            return v_[7];
            break;
        case 3:
            return v_[3];
            break;
        }
        break;
    case 5:
        switch (i) {
        case 0:
            return v_[4];
            break;
        case 1:
            return v_[7];
            break;
        case 2:
            return v_[6];
            break;
        case 3:
            return v_[5];
            break;
        }
        break;
    }
}

template<class T>
const Coordinate<T,3>* Hexahedron8<T>::getSideVertex(const UInt f,
                                              const UInt i) const {
	return getSideV(f,i);
}

template<class T>
Real Hexahedron8<T>::getAreaOfFace(const UInt f) const {
    CartesianVector<T,3> v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((Real) 0.5 * (v1 ^ v2).norm());
}

template<class T>
Real Hexahedron8<T>::getVolume() const {
    throw ErrorNotImplemented("Hexa8::getVolume()");
}

template<class T>
void Hexahedron8<T>::setV(const UInt i, const Coordinate<T,3>* coord) {
    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
ElemI* Hexahedron8<T>::toStructured(const GroupCoordinates<CoordI3>& cG,
                              const Grid3& grid, const Real tol) const {
    CoordinateId* vIds = this->vertexToStructured(cG, grid, tol);
    if (vIds == NULL) {
        return NULL;
    }
    ElemI* res =  new HexI8(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete[] vIds;
    return res;
}

template<class T>
ElemR* Hexahedron8<T>::toUnstructured(const GroupCoordinates<CoordR3>& cG,
                                const Grid3& grid) const {
    CoordinateId* vIds = this->vertexToUnstructured(cG, grid);
    if (vIds == NULL) {
        return NULL;
    }
    ElemR* res =  new HexR8(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete[] vIds;
    return res;
}

template<class T>
void Hexahedron8<T>::printInfo() const {
    cout << "--- Hex8 Info ---" << endl;
    cout << "Id: " << this->getId() << endl;
    cout << "Coordinates:" << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        getV(i)->printInfo();
    }
    if (isRegular()) {
        cout << "Is regular Hex." << endl;
    } else {
        cout << "Is not a regular Hex." << endl;
    }
}

template class Hexahedron8<Real>;
template class Hexahedron8<Int >;

