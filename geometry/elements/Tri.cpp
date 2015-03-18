#ifndef TRI_H_
#include "Tri.h"
#endif

Tri::Tri() {

}

Tri::~Tri() {

}

void Tri::getOrderedVerticesId(UInt val[3]) const {
	for (UInt i = 0; i < 3; i++) {
		val[i] = getVertex(i)->getId();
	}
	ascendingOrder(3, val);
}
