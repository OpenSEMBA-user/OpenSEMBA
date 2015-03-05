/*
 * Polyhedron.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: luis
 */

#include "Polyhedron.h"

Polyhedron::Polyhedron(const ElementId id,
                       const vector<const Polygon*>& face,
                       const uint layerId,
                       const uint matId)
:   Volume(id, layerId, matId) {
    face_ = face;
    for (uint f = 0; f < numberOfFaces(); f++) {
        for (uint i = 0; i < face[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
    checkClosedness();
}

Polyhedron::Polyhedron(const Polyhedron& rhs)
:   Volume(rhs) {

    face_ = rhs.face_;
    for (uint f = 0; f < numberOfFaces(); f++) {
        for (uint i = 0; i < face_[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
}

Polyhedron::Polyhedron(const ElementId id, const Polyhedron& rhs)
:   Volume(id, rhs) {

    face_ = rhs.face_;
    for (uint f = 0; f < numberOfFaces(); f++) {
        for (uint i = 0; i < face_[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
}

Polyhedron::~Polyhedron() {

}

ClassBase* Polyhedron::clone() const {
    return new Polyhedron(*this);
}

ClassBase* Polyhedron::clone(const ElementId id) const {
    return new Polyhedron(id, *this);
}

bool Polyhedron::isCurvedFace(const uint f) const {
    return getFace(f)->isCurved();
}

uint Polyhedron::numberOfCoordinates() const {
    return v_.size();
}

uint Polyhedron::numberOfVertices() const {
    return numberOfCoordinates();
}

uint Polyhedron::numberOfFaces() const {
    return face_.size();
}

uint Polyhedron::numberOfSideVertices(const uint f) const {
    assert(f < numberOfFaces());
    return face_[f]->numberOfVertices();
}

uint Polyhedron::numberOfSideCoordinates(const uint f) const {
    return numberOfSideVertices(f);
}

const CoordD3* Polyhedron::getV(
const uint i) const {
    assert(i < numberOfCoordinates());
    return v_[i];
}

const CoordD3* Polyhedron::getSideV(
const uint f,
const uint i) const {
    return getFace(f)->getV(i);
}

const CoordD3* Polyhedron::getVertex(const uint i) const {
    return getV(i);
}

const CoordD3* Polyhedron::getSideVertex(const uint f, const uint i) const {
    return getSideV(f,i);
}

const Polygon* Polyhedron::getFace(const uint f) const {
    assert(f < numberOfFaces());
    return face_[f];
}

double Polyhedron::getAreaOfFace(const uint f) const {
    return getFace(f)->getArea();
}

void Polyhedron::printInfo() const {
    cout << "--- Polyhedron info ---" << endl
         << "Number of vertices: " << numberOfVertices() << endl
         << "Number of faces: " << numberOfFaces() << endl;
    for (uint i = 0; i < numberOfFaces(); i++) {
        cout<< "Face #" << i << endl;
        getFace(i)->printInfo();
    }
}

void Polyhedron::addV(const Coordinate<double, 3>* v) {
    for (uint i = 0; i < numberOfCoordinates(); i++) {
        if (*v_[i] == *v) {
            return;
        }
    }
    v_.push_back(v);
}

void Polyhedron::checkClosedness() const {
    // This checks consists in pairing segments ordered according to their ids.
    // If a segment is not paired, the surface is not closed.
    uint nSidesInFaces = 0;
    for (uint f = 0; f < numberOfFaces(); f++) {
        nSidesInFaces += getFace(f)->numberOfFaces();
    }
    DynMatrix<uint> list(nSidesInFaces, 2);
    uint row = 0;
    for (uint f = 0; f < numberOfFaces(); f++) {
        for (uint s = 0; s < getFace(f)->numberOfFaces(); s++) {
            uint id0 = getFace(f)->getSideVertex(s,0)->getId();
            uint id1 = getFace(f)->getSideVertex(s,1)->getId();
            if (id0 < id1) {
                list(row,0) = id0;
                list(row,1) = id1;
            } else {
                list(row,0) = id1;
                list(row,1) = id0;
            }
            row++;
        }
    }
    list.sortRows(0,1);
    for (uint i = 0; i < nSidesInFaces; i += 2) {
        if (list(i,0) != list(i+1,0) || list(i,1) != list(i+1,1)) {
            cerr << "ERROR @ Polyhedron: "
                 << "Polyhedron is not closed." << endl;
            printInfo();
        }
    }
}
