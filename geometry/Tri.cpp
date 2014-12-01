#ifndef TRI_H_
#include "Tri.h"
#endif

Tri::Tri() {

}

Tri::~Tri() {

}

void
Tri::getOrderedVerticesId(unsigned int val[3]) const {
	for (unsigned int i = 0; i < 3; i++) {
		val[i] = getVertex(i)->id;
	}
	ascendingOrder(3, val);
}
