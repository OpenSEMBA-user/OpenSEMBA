/*
 * Polygon.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#include "Polygon.h"

Polygon::Polygon() {

}

Polygon::Polygon(const CoordinateGroup<>& cG,
                 const ElementId id,
                 const vector<CoordinateId>& vId,
                 const LayerId layerId,
                 const uint matId)
:   Surface(id, layerId, matId) {
    
	assert(vId.size() >= 3);
	v_.resize(vId.size());
	for (uint i = 0; i < vId.size(); i++) {
		const CoordinateBase* coord = cG.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Polygon::Polygon(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        if (!coord->is<CoordD3>()) {
            cerr << "ERROR @ Polygon::Polygon(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
            assert(false);
            exit(ELEMENT_ERROR);
        }
        v_[i] = coord->castTo<CoordD3>();
	}
}

Polygon::Polygon(const Polygon& rhs)
:   Surface(rhs) {
    
    v_ = rhs.v_;
}

Polygon::Polygon(const ElementId id, const Polygon& rhs)
:   Surface(id, rhs) {
    
    v_ = rhs.v_;
}

Polygon::~Polygon() {
    
}

ClassBase* Polygon::clone() const {
    return new Polygon(*this);
}

ClassBase* Polygon::clone(const ElementId id) const {
    return new Polygon(id, *this);
}

uint Polygon::numberOfFaces() const {
	return v_.size();
}

uint Polygon::numberOfVertices() const {
	return numberOfCoordinates();
}

uint Polygon::numberOfCoordinates() const {
	return v_.size();
}

uint
Polygon::numberOfSideVertices(const uint f = 0) const {
	return 2;
}

uint
Polygon::numberOfSideCoordinates(const uint f = 0) const {
	return 2;
}

const CoordD3* Polygon::getV(const uint i) const {
	assert(i < numberOfCoordinates());
	return v_[i];
}

const CoordD3* Polygon::getSideV(const uint f,
                                 const uint i) const {
    
	return v_[(f + i) % numberOfCoordinates()];
}

const CoordD3* Polygon::getVertex(const uint i) const {
	return getV(i);
}

const CoordD3* Polygon::getSideVertex(const uint f, const uint i) const {
	return getSideV(f,i);
}

double Polygon::getArea() const {
	cerr<< "ERROR @ Polygon getArea: "
		<< "Not implemented" << endl;
	exit(ELEMENT_ERROR);
}

void Polygon::setV(const uint i, const CoordD3* coord) {
	assert(i < numberOfCoordinates());
	v_[i] = coord;
}

void Polygon::printInfo() const {
	cout<< "--- Polygon info ---" << endl
		<< "Number of coordinates: " << numberOfCoordinates() << endl;
	cout<< "Id: " << getId() << ", MatId: " << getMatId() << endl;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		cout<< "#" << i << ": ";
		v_[i]->printInfo();
		cout<< endl;
	}
	cout<< "--- End of polygon info ---" << endl;
}
