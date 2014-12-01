/*
 * MeshSurface.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: luis
 */

#include "MeshSurface.h"

MeshSurface::MeshSurface(
 const vector<CartesianVector<double, 3> >& normal,
 const CoordinateGroup coordinates,
 const unsigned int numberOfElements) {
	assert(normal.size() == numberOfElements);
	v = coordinates;
	unsigned int id = 1;
	unsigned int matId = 0;
	unsigned int auxVId = 1;
	vector<Tri3> tri3;
	vector<Tri6> tri6;
	if (v.size() / 3 == numberOfElements) {
		tri3.reserve(numberOfElements);
		for (unsigned int i = 0; i < numberOfElements; i++) {
			unsigned int vId[3];
			for (unsigned int j = 0; j < 3; j++) {
				vId[j] = auxVId++;
			}
			tri3.push_back(
			 Tri3(v, id++, matId, vId, normal[i]));
		}
	} else if (v.size() / 6 == numberOfElements) {
		tri6.reserve(numberOfElements);
		for (unsigned int i = 0; i < numberOfElements; i++) {
			unsigned int vId[6];
			for (unsigned int j = 0; j < 6; j++) {
				vId[j] = auxVId++;
			}
			tri6.push_back(
			 Tri6(v, id++, matId, vId, normal[i]));
		}
	} else {
		cout << "ERROR @ MeshSurface ctor." << endl;
		cout << "Number of coordinates given mismatch." << endl;
		exit(-1);
	}
	elem = ElementsGroup(tri3, tri6);
	elem.reassignPointers(v);
	map.build(v, elem);
}

MeshSurface::~MeshSurface() {
	// TODO Auto-generated destructor stub
}

