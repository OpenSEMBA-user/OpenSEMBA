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

Lin2::Lin2(
 const CoordinateGroup<>& coordGr,
 const CoordinateId vId[2],
 const uint id_,
 const uint matId_,
 const uint layerId_) :
         Line(id_, matId_, layerId_) {
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Lin2::Lin2(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->isOf<CoordD3>()) {
            cerr << "ERROR @ Lin2::Lin2(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v[i] = coord->castTo<CoordD3>();
	}
}

Lin2::Lin2(
 const CoordD3* v_[2],
 const uint id_,
 const uint matId_,
 const uint layerId_) :
         Line(id_, matId_, layerId_) {
	for (uint i = 0; i < lin.np; i++) {
		v[i] = v_[i];
	}
}


Lin2::~Lin2() {

}

const CoordD3*
Lin2::getVertex(const uint i) const {
	return v[i];
}

void
Lin2::setV(
 const uint i,
 const CoordD3* constCoordinate) {
	assert(i < numberOfCoordinates());
	v[i] = constCoordinate;
}

void
Lin2::printInfo() const {
	cout << "--- Lin2 info ---" << endl;
	cout << "Id: " << getId() << endl;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}



