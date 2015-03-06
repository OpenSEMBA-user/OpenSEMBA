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
             const uint matId)
:   Quad(id, layerId, matId) {
    
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = cG.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Quad4::Quad4(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->is<CoordD3>()) {
            cerr << "ERROR @ Quad4::Quad4(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordD3>();
	}
	// TODO Normals are not handled.
	check();
}

Quad4::Quad4(const Quad4& rhs)
:   Quad(rhs) {
    
    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Quad4::Quad4(const ElementId id, const Quad4& rhs)
:   Quad(id, rhs) {
    
    for (uint i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

Quad4::~Quad4() {
    
}

ClassBase* Quad4::clone() const {
    return new Quad4(*this);
}

ClassBase* Quad4::clone(const ElementId id) const {
    return new Quad4(id, *this);
}

const CoordD3* Quad4::getVertex(const uint i) const {
	return v_[i];
}

const CoordD3* Quad4::getSideV(const uint f,
                               const uint i) const {
	assert(f < numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v_[(f + i) % 4];
}

const CoordD3* Quad4::getSideVertex(const uint f,
                                    const uint i) const {
	assert(f < numberOfFaces());
	assert(i < numberOfSideVertices());
	return v_[(f + i) % 4];
}

void Quad4::setV(const uint i, const CoordD3* coord) {
    v_[i] = coord;
}

void Quad4::printInfo() const {
	cout << "--- Quad4 info ---" << endl;
	Quad::printInfo();
}

void Quad4::check() const {
	// TODO Auto-generated
}
