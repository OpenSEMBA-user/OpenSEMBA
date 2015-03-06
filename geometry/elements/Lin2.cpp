/*
 * Lin2.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef LIN2_H_
#include "Lin2.h"
#endif

Lin2::Lin2() {

}

Lin2::Lin2(const CoordinateGroup<>& coordGr,
           const ElementId id,
           const CoordinateId vId[2],
           const LayerId layerId,
           const uint matId)
:   Line(id, layerId, matId) {
    
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Lin2::Lin2(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->is<CoordD3>()) {
            cerr << "ERROR @ Lin2::Lin2(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordD3>();
	}
}

Lin2::Lin2(const ElementId id,
           const CoordD3* v[2],
           const LayerId layerId,
           const uint matId)
:   Line(id, layerId, matId) {
    
	for (uint i = 0; i < lin.np; i++) {
		v_[i] = v[i];
	}
}

Lin2::Lin2(const Lin2& rhs)
:   Line(rhs) {
    
    for (uint i = 0; i < lin.np; i++) {
		v_[i] = rhs.v_[i];
	}
}

Lin2::Lin2(const ElementId id,
           const Lin2& rhs)
:   Line(id, rhs) {
    
    for (uint i = 0; i < lin.np; i++) {
		v_[i] = rhs.v_[i];
	}
}

Lin2::~Lin2() {

}

ClassBase* Lin2::clone() const {
    return new Lin2(*this);
}

ClassBase* Lin2::clone(const ElementId id) const {
    return new Lin2(id, *this);
}

const CoordD3* Lin2::getSideV(const uint f, const uint i) const {
    return v_[i];
}

const CoordD3* Lin2::getVertex(const uint i) const {
	return v_[i];
}

const CoordD3* Lin2::getSideVertex(const uint f, const uint i) const {
    return v_[i];
}

void Lin2::setV(const uint i, const CoordD3* coord) {
    
	assert(i < numberOfCoordinates());
	v_[i] = coord;
}

void Lin2::printInfo() const {
	cout << "--- Lin2 info ---" << endl;
	cout << "Id: " << getId() << endl;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		v_[i]->printInfo();
		cout << endl;
	}
}
