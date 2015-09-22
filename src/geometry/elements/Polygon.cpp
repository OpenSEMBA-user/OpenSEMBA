// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * Polygon.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#include "Polygon.h"

Polygon::Polygon() {

}

Polygon::Polygon(const GroupCoordinates<CoordR3>& cG,
                 const ElementId id,
                 const vector<CoordinateId>& vId,
                 const LayerId layerId,
                 const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
    const UInt vSize = vId.size();
	assert(vId.size() >= 5); // Polygons of 3 or 4 vertices are treated as triangles or quads.
	v_.resize(vSize);
	for (UInt i = 0; i < vSize; i++) {
        v_[i] = cG.getId(vId[i]);
	}
}

Polygon::Polygon(const Polygon& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {
    
    v_ = rhs.v_;
}

Polygon::~Polygon() {
    
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
    throw ErrorNotImplemented("Polygon::getArea");
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
	}
	cout<< "--- End of polygon info ---" << endl;
}
