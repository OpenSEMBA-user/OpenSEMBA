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
                 const MatId   matId)
:   Surface(id, layerId, matId) {
    
	assert(vId.size() >= 3);
	v_.resize(vId.size());
	for (UInt i = 0; i < vId.size(); i++) {
		const CoordinateBase* coord = cG.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << endl << "ERROR @ Polygon::Polygon(): "
                 << "Coord in new CoordinateGroup inexistent"
                 << endl;
        }
        if (!coord->is<CoordR3>()) {
            cerr << endl << "ERROR @ Polygon::Polygon(): "
                 << "Coord in new CoordinateGroup is not a valid Coord"
                 << endl;
        }
        v_[i] = coord->castTo<CoordR3>();
	}
}

Polygon::Polygon(const Polygon& rhs)
:   Surface(rhs) {
    
    v_ = rhs.v_;
}

Polygon::~Polygon() {
    
}

ClassBase* Polygon::clone() const {
    return new Polygon(*this);
}

UInt Polygon::numberOfFaces() const {
	return v_.size();
}

UInt Polygon::numberOfVertices() const {
	return numberOfCoordinates();
}

UInt Polygon::numberOfCoordinates() const {
	return v_.size();
}

UInt
Polygon::numberOfSideVertices(const UInt f = 0) const {
	return 2;
}

UInt
Polygon::numberOfSideCoordinates(const UInt f = 0) const {
	return 2;
}

const CoordR3* Polygon::getV(const UInt i) const {
	assert(i < numberOfCoordinates());
	return v_[i];
}

const CoordR3* Polygon::getSideV(const UInt f,
                                 const UInt i) const {
    
	return v_[(f + i) % numberOfCoordinates()];
}

const CoordR3* Polygon::getVertex(const UInt i) const {
	return getV(i);
}

const CoordR3* Polygon::getSideVertex(const UInt f, const UInt i) const {
	return getSideV(f,i);
}

Real Polygon::getArea() const {
	cerr << endl << "ERROR @ Polygon getArea: Not implemented" << endl;
	return 0.0;
}

void Polygon::setV(const UInt i, const CoordR3* coord) {
	assert(i < numberOfCoordinates());
	v_[i] = coord;
}

void Polygon::printInfo() const {
	cout<< "--- Polygon info ---" << endl
		<< "Number of coordinates: " << numberOfCoordinates() << endl;
	cout<< "Id: " << getId() << ", MatId: " << getMatId() << endl;
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		cout<< "#" << i << ": ";
		v_[i]->printInfo();
		cout<< endl;
	}
	cout<< "--- End of polygon info ---" << endl;
}
