#ifndef TRI_H_
#include "Tri.h"
#endif

Tri::Tri() {

}

Tri::Tri(
 const CVecD3& normal_,
 const uint id_, const uint matId_, const uint layerId_) :
         Surface(normal_, id_, matId_, layerId_) {
}

Tri::~Tri() {

}

void
Tri::getOrderedVerticesId(unsigned int val[3]) const {
	for (unsigned int i = 0; i < 3; i++) {
		val[i] = getVertex(i)->getId();
	}
	ascendingOrder(3, val);
}

Tri&
Tri::operator =(const Tri& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Surface::operator=(rhs);
    return *this;
}
