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
           const MatId   matId)
:   Line(id, layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Lin2::Lin2(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        if (!coord->is<CoordR3>()) {
            cerr << "ERROR @ Lin2::Lin2(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        v_[i] = coord->castTo<CoordR3>();
	}
}

Lin2::Lin2(const ElementId id,
           const CoordR3* v[2],
           const LayerId layerId,
           const MatId   matId)
:   Line(id, layerId, matId) {
    
	for (UInt i = 0; i < lin.np; i++) {
		v_[i] = v[i];
	}
}

Lin2::Lin2(const Lin2& rhs)
:   Line(rhs) {
    
    for (UInt i = 0; i < lin.np; i++) {
		v_[i] = rhs.v_[i];
	}
}

Lin2::~Lin2() {

}

ClassBase* Lin2::clone() const {
    return new Lin2(*this);
}

const CoordR3* Lin2::getSideV(const UInt f, const UInt i) const {
    return v_[i];
}

const CoordR3* Lin2::getVertex(const UInt i) const {
	return v_[i];
}

const CoordR3* Lin2::getSideVertex(const UInt f, const UInt i) const {
    return v_[i];
}

void Lin2::setV(const UInt i, const CoordR3* coord) {
    
	assert(i < numberOfCoordinates());
	v_[i] = coord;
}

void Lin2::printInfo() const {
	cout << "--- Lin2 info ---" << endl;
	cout << "Id: " << getId() << endl;
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		v_[i]->printInfo();
		cout << endl;
	}
}
