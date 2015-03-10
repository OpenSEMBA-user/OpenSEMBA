/*
 * Hex8.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#include "Hex8.h"

const Real Hex8::tolerance = 1e-15;

Hex8::Hex8() {

}

Hex8::Hex8(const CoordinateGroup<>& coordGr,
           const ElementId id,
           const CoordinateId vId[8],
           const LayerId layerId,
           const MatId   matId)
:   Volume(id, layerId, matId) {

	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Hex8::Hex8(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->is<CoordR3>()) {
            cerr << "ERROR @ Hex8::Hex8(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordR3>();
	}
}

Hex8::Hex8(const CoordinateGroup<>& cG,
           const ElementId id,
           const CVecR3& min,
           const CVecR3& max,
           const LayerId layerId,
           const MatId   matId)
:   Volume(id, layerId, matId) {

    v_[0] = cG.get(CVecR3(min(0), min(1), min(2)));
    v_[1] = cG.get(CVecR3(min(0), min(1), max(2)));
    v_[2] = cG.get(CVecR3(min(0), max(1), min(2)));
    v_[3] = cG.get(CVecR3(min(0), max(1), max(2)));
    v_[4] = cG.get(CVecR3(max(0), min(1), min(2)));
    v_[5] = cG.get(CVecR3(max(0), min(1), max(2)));
    v_[6] = cG.get(CVecR3(max(0), max(1), min(2)));
    v_[7] = cG.get(CVecR3(max(0), max(1), max(2)));
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		if (v_[i] == NULL) {
			cerr<< "ERROR @ Hex8 ctor: "
				<< "Coordinate " << i << " pointer is null." << endl;
            assert(false);
            exit(ELEMENT_ERROR);
		}
	}
}

Hex8::Hex8(const Hex8& rhs)
:   Volume(rhs) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Hex8::~Hex8() {

}

ClassBase* Hex8::clone() const {
    return new Hex8(*this);
}

bool Hex8::isRegular() const {
    // Checks that all edges are aligned with one of the axis.
    static const CVecR3 xAxe(1.0, 0.0, 0.0);
    static const CVecR3 yAxe(0.0, 1.0, 0.0);
    static const CVecR3 zAxe(1.0, 0.0, 1.0);
    for (UInt f = 0; f < numberOfFaces(); f++) {
        CVecR3 first, second;
        CVecR3 inc;
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

const CoordR3* Hex8::getSideV(const UInt f, const UInt i) const {
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

const CoordR3* Hex8::getSideVertex(const UInt f, const UInt i) const {
	return getSideV(f,i);
}

Real Hex8::getAreaOfFace(const UInt f) const {
    CartesianVector<Real,3> v1, v2;
    v1 = getSideV(f,1)->pos() - getSideV(f,0)->pos();
    v2 = getSideV(f,2)->pos() - getSideV(f,0)->pos();
    return ((Real) 0.5 * (v1 ^ v2).norm());
}

void Hex8::setV(const UInt i, const CoordR3* coord) {
	assert(i < numberOfCoordinates());
	v_[i] = coord;
}

void Hex8::printInfo() const {
	cout << "--- Hex8 Info ---" << endl;
	cout << "Id: " << getId() << endl;
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

