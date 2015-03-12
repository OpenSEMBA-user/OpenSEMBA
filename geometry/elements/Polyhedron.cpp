/*
 * Polyhedron.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: luis
 */

#include "Polyhedron.h"

Polyhedron::Polyhedron() {

}

Polyhedron::Polyhedron(const ElementId id,
                       const vector<const Polygon*>& face,
                       const LayerId layerId,
                       const MatId   matId)
:   Volume<Real>(id, layerId, matId) {
    face_ = face;
    for (UInt f = 0; f < numberOfFaces(); f++) {
        for (UInt i = 0; i < face[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
    checkClosedness();
}

Polyhedron::Polyhedron(const Polyhedron& rhs)
:   Volume<Real>(rhs) {

    face_ = rhs.face_;
    for (UInt f = 0; f < numberOfFaces(); f++) {
        for (UInt i = 0; i < face_[f]->numberOfCoordinates(); i++) {
            addV(face_[f]->getV(i));
        }
    }
}

Polyhedron::~Polyhedron() {

}

ClassBase* Polyhedron::clone() const {
    return new Polyhedron(*this);
}

bool Polyhedron::isCurvedFace(const UInt f) const {
    return getFace(f)->isCurved();
}

UInt Polyhedron::numberOfCoordinates() const {
    return v_.size();
}

UInt Polyhedron::numberOfVertices() const {
    return numberOfCoordinates();
}

UInt Polyhedron::numberOfFaces() const {
    return face_.size();
}

UInt Polyhedron::numberOfSideVertices(const UInt f) const {
    assert(f < numberOfFaces());
    return face_[f]->numberOfVertices();
}

UInt Polyhedron::numberOfSideCoordinates(const UInt f) const {
    return numberOfSideVertices(f);
}

const CoordR3* Polyhedron::getV(
const UInt i) const {
    assert(i < numberOfCoordinates());
    return v_[i];
}

const CoordR3* Polyhedron::getSideV(
const UInt f,
const UInt i) const {
    return getFace(f)->getV(i);
}

const CoordR3* Polyhedron::getVertex(const UInt i) const {
    return getV(i);
}

const CoordR3* Polyhedron::getSideVertex(const UInt f, const UInt i) const {
    return getSideV(f,i);
}

const Polygon* Polyhedron::getFace(const UInt f) const {
    assert(f < numberOfFaces());
    return face_[f];
}

Real Polyhedron::getAreaOfFace(const UInt f) const {
    return getFace(f)->getArea();
}

void Polyhedron::printInfo() const {
    cout << "--- Polyhedron info ---" << endl
         << "Number of vertices: " << numberOfVertices() << endl
         << "Number of faces: " << numberOfFaces() << endl;
    for (UInt i = 0; i < numberOfFaces(); i++) {
        cout<< "Face #" << i << endl;
        getFace(i)->printInfo();
    }
}

void Polyhedron::addV(const Coordinate<Real, 3>* v) {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        if (*v_[i] == *v) {
            return;
        }
    }
    v_.push_back(v);
}

void Polyhedron::checkClosedness() const {
    // This checks consists in pairing segments ordered according to their ids.
    // If a segment is not paired, the surface is not closed.
    UInt nSidesInFaces = 0;
    for (UInt f = 0; f < numberOfFaces(); f++) {
        nSidesInFaces += getFace(f)->numberOfFaces();
    }
    DynMatrix<UInt> list(nSidesInFaces, 2);
    UInt row = 0;
    for (UInt f = 0; f < numberOfFaces(); f++) {
        for (UInt s = 0; s < getFace(f)->numberOfFaces(); s++) {
            UInt id0 = getFace(f)->getSideVertex(s,0)->getId();
            UInt id1 = getFace(f)->getSideVertex(s,1)->getId();
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
    for (UInt i = 0; i < nSidesInFaces; i += 2) {
        if (list(i,0) != list(i+1,0) || list(i,1) != list(i+1,1)) {
            cerr << endl << "ERROR @ Polyhedron: "
                 << "Polyhedron is not closed." << endl;
            printInfo();
        }
    }
}
