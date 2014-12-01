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
 const CoordinateGroup& coordGr,
 const unsigned int id_,
 const unsigned int matId_,
 const unsigned int vId[2]) {
	id = id_;
	matId = matId_;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i] = coordGr.getPtrToId(vId[i]);
	}
}

Lin2::Lin2(
 const unsigned int id_,
 const unsigned int matId_,
 const Coordinate<double,3>* v_[2]) {
	id = id_;
	matId = matId_;
	for (unsigned int i = 0; i < lin.np; i++) {
		v[i] = v_[i];
	}
}


Lin2::~Lin2() {

}

const Coordinate<double,3>*
Lin2::getVertex(const unsigned int i) const {
	return v[lin.vertex(i)];
}

void
Lin2::setV(
 const unsigned int i,
 const Coordinate<double, 3>* constCoordinate) {
	assert(i < numberOfCoordinates());
	v[i] = constCoordinate;
}

void
Lin2::printInfo() const {
	cout << "--- Lin2 info ---" << endl;
	cout << "Id: " << getId() << endl;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		v[i]->printInfo();
		cout << endl;
	}
}



