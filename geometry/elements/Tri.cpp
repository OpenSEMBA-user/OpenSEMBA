#ifndef TRI_H_
#include "Tri.h"
#endif

Tri::Tri(const LayerId layerId,
         const MatId   matId)
:   SurfR(layerId, matId) {

}

Tri::Tri(const ElementId id,
         const LayerId layerId,
         const MatId   matId)
:   SurfR(id, layerId, matId) {

}

Tri::Tri(const Tri& rhs)
:   SurfR(rhs) {

}

Tri::~Tri() {

}

void Tri::getOrderedVerticesId(UInt val[3]) const {
	for (UInt i = 0; i < 3; i++) {
		val[i] = getVertex(i)->getId();
	}
	ascendingOrder(3, val);
}
