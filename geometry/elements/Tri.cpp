#ifndef TRI_H_
#include "Tri.h"
#endif

Tri::Tri(const LayerId layerId,
         const MatId   matId)
:   Surface(layerId, matId) {

}

Tri::Tri(const ElementId id,
         const LayerId layerId,
         const MatId   matId)
:   Surface(id, layerId, matId) {

}

Tri::Tri(const Tri& rhs)
:   Surface(rhs) {

}

Tri::~Tri() {

}

void Tri::getOrderedVerticesId(uint val[3]) const {
	for (uint i = 0; i < 3; i++) {
		val[i] = getVertex(i)->getId();
	}
	ascendingOrder(3, val);
}
