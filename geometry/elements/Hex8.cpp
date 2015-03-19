/*
 * Hex8.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#include "Hex8.h"

template<class T>
const Real Hex8<T>::tolerance = 1e-15;

template<class T>
Hex8<T>::Hex8() {

}

template<class T>
Hex8<T>::Hex8(const CoordinateGroup<Coordinate<T,3> >& coordGr,
              const ElementId id,
              const CoordinateId vId[8],
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {

	for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = coordGr.getPtrToId(vId[i]);
	}
}

template<class T>
Hex8<T>::Hex8(CoordinateGroup<Coordinate<T,3> >& cG,
              const ElementId id,
              const Box<T,3>& box,
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {

    if(!box.isVolume()) {
        cerr << endl << "ERROR @ Hex8::Hex8(): "
                     << "Box is not a Volume" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }
    vector<CartesianVector<T,3> > pos = box.getPos();
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
	    v_[i] = cG.get(pos[i]);
		if (v_[i] == NULL) {
			v_[i] = cG.add(pos[i]);
		}
	}
}

template<class T>
Hex8<T>::Hex8(const Hex8<T>& rhs)
:   Elem(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
Hex8<T>::~Hex8() {

}

template<class T>
ClassBase* Hex8<T>::clone() const {
    return new Hex8<T>(*this);
}

template<class T>
bool Hex8<T>::isStructured(const Grid3& grid, const Real tol) const {
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        if (!grid.isCell(*this->getV(i), tol)) {
            return false;
        }
    }
    Box<T,3> bound(*this->getMinV(), *this->getMaxV());
    if (!bound.isVolume()) {
        return false;
    }
    vector< CartesianVector<T,3> > pos = bound.getPos();
    vector<bool> found(pos.size(), false);
    if (pos.size() != this->numberOfCoordinates()) {
        return false;
    }
    bool foundCoord;
    for(UInt i = 0; i < this->numberOfCoordinates(); i++) {
        foundCoord = false;
        for(UInt j= 0; j < pos.size(); j++) {
            if (found[j]) {
                continue;
            }
            if (*this->getV(i) == pos[j]) {
                foundCoord = true;
                found[j] = true;
                break;
            }
        }
        if (!foundCoord) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Hex8<T>::isRegular() const {
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
const Coordinate<T,3>* Hex8<T>::getSideV(const UInt f, const UInt i) const {
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
    cerr << "ERROR @ Hex8" << endl;
    cerr << "Side vertex not found." << endl;
    exit(-1);
}

template<class T>
const Coordinate<T,3>* Hex8<T>::getSideVertex(const UInt f,
                                              const UInt i) const {
	return getSideV(f,i);
}

template<class T>
Real Hex8<T>::getAreaOfFace(const UInt f) const {
    CartesianVector<T,3> v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((Real) 0.5 * (v1 ^ v2).norm());
}

template<class T>
void Hex8<T>::setV(const UInt i, const Coordinate<T,3>* coord) {
    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
ElemI* Hex8<T>::toStructured(CoordinateGroup<CoordI3>& cG,
                             const Grid3& grid, const Real tol) const {
    if (this->template is<ElemI>()) {
        return NULL;
    }
    if (!isStructured(grid, tol)) {
        return NULL;
    }
    CVecI3 cell;
    const CoordI3* coord;
    CoordinateId coordId;
    CoordinateId* vIds = new CoordinateId[this->numberOfCoordinates()];
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        cell  = grid.getCell(*this->getV(i));
        coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            cG.add(new CoordI3(coordId, cell));
        }
        coord = cG.getPtrToId(coordId);
        if (*coord != cell) {
            cerr << endl << "ERROR @ Element::toStructured(): "
                 << "Existent Coordinate not coincident." << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        vIds[i] = coordId;
    }
    ElemI* res =  new HexI8(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete vIds;
    return res;
}

template<class T>
void Hex8<T>::printInfo() const {
    cout << "--- Hex8 Info ---" << endl;
    cout << "Id: " << this->getId() << endl;
    cout << "Coordinates:" << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        getV(i)->printInfo();
        cout << endl;
    }
    if (isRegular()) {
        cout << "Is regular Hex." << endl;
    } else {
        cout << "Is not a regular Hex." << endl;
    }
}

template class Hex8<Real>;
template class Hex8<Int >;

