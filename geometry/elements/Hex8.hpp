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
Hex8<T>::Hex8(const CoordinateGroup<>& coordGr,
              const ElementId id,
              const CoordinateId vId[8],
              const LayerId layerId,
              const MatId   matId)
:   Volume<T>(id, layerId, matId) {

	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Hex8<T>::Hex8(): "
                 << "Coordinate in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        if (!coord->is< Coordinate<T,3> >()) {
            cerr << "ERROR @ Hex8<T>::Hex8(): "
                 << "Coordinate in new CoordinateGroup is not a valid Coordinate"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        v_[i] = coord->castTo< Coordinate<T,3> >();
	}
}

template<class T>
Hex8<T>::Hex8(const CoordinateGroup<>& cG,
              const ElementId id,
              const CartesianVector<T,3>& min,
              const CartesianVector<T,3>& max,
              const LayerId layerId,
              const MatId   matId)
:   Volume<T>(id, layerId, matId) {

    v_[0] = cG.get(CartesianVector<T,3>(min(0), min(1), min(2)));
    v_[1] = cG.get(CartesianVector<T,3>(min(0), min(1), max(2)));
    v_[2] = cG.get(CartesianVector<T,3>(min(0), max(1), min(2)));
    v_[3] = cG.get(CartesianVector<T,3>(min(0), max(1), max(2)));
    v_[4] = cG.get(CartesianVector<T,3>(max(0), min(1), min(2)));
    v_[5] = cG.get(CartesianVector<T,3>(max(0), min(1), max(2)));
    v_[6] = cG.get(CartesianVector<T,3>(max(0), max(1), min(2)));
    v_[7] = cG.get(CartesianVector<T,3>(max(0), max(1), max(2)));
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		if (v_[i] == NULL) {
			cerr<< "ERROR @ Hex8 ctor: "
				<< "Coordinate " << i << " pointer is null." << endl;
            assert(false);
            exit(EXIT_FAILURE);
		}
	}
}

template<class T>
Hex8<T>::Hex8(const Hex8<T>& rhs)
:   Volume<T>(rhs) {

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
const Coordinate<T,3>* Hex8<T>::getSideVertex(const UInt f, const UInt i) const {
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

