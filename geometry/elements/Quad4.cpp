/*
 * Quad4.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#include "Quad4.h"

Quad4::Quad4() {

}

Quad4::Quad4(const CoordinateGroup<>& cG,
             const ElementId id,
             const CoordinateId vId[4],
             const LayerId layerId,
             const MatId   matId)
:   Quad(id, layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = cG.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Quad4::Quad4(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        if (!coord->is<CoordR3>()) {
            cerr << "ERROR @ Quad4::Quad4(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        v_[i] = coord->castTo<CoordR3>();
	}
	// TODO Normals are not handled.
	check();
}

Quad4::Quad4(const Quad4& rhs)
:   Quad(rhs) {
    
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Quad4::~Quad4() {
    
}

ClassBase* Quad4::clone() const {
    return new Quad4(*this);
}

const CoordR3* Quad4::getVertex(const UInt i) const {
	return v_[i];
}

const CoordR3* Quad4::getSideV(const UInt f,
                               const UInt i) const {
	assert(f < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v_[(f + i) % 4];
}

const CoordR3* Quad4::getSideVertex(const UInt f,
                                    const UInt i) const {
	assert(f < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v_[(f + i) % 4];
}

void Quad4::setV(const UInt i, const CoordR3* coord) {
    v_[i] = coord;
}

void Quad4::printInfo() const {
	cout << "--- Quad4 info ---" << endl;
	Quad::printInfo();
}

void Quad4::check() const {
	// TODO Auto-generated
}
