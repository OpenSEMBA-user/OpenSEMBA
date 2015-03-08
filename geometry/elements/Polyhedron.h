/*
 * Polyhedron.h
 *
 *  Created on: Jul 24, 2014
 *      Author: luis
 */

#ifndef POLYHEDRON_H_
#define POLYHEDRON_H_

#include "Volume.h"
#include "Polygon.h"

class Polyhedron : public Volume {
public:
    Polyhedron();
    Polyhedron(const ElementId id,
               const vector<const Polygon*>& faces,
               const LayerId layerId = LayerId(0),
               const uint matId   = 0);
    Polyhedron(const Polyhedron& rhs);
    virtual ~Polyhedron();

    ClassBase* clone() const;

    bool isCurvedFace(const uint f) const;

    uint numberOfFaces      () const;
    uint numberOfVertices   () const;
    uint numberOfCoordinates() const;

    uint numberOfSideVertices   (const uint f = 0) const;
    uint numberOfSideCoordinates(const uint f = 0) const;
    const CoordD3* getV(const uint i) const;
    const CoordD3* getSideV(const uint f, const uint i) const;
    const CoordD3* getVertex(const uint i) const;
    const CoordD3* getSideVertex(const uint f, const uint i) const;

    const Polygon* getFace(const uint f) const;
    double getAreaOfFace(const uint f) const;

    void printInfo() const;

private:
    vector<const CoordD3*> v_;
    vector<const Polygon*> face_;

    void addV(const CoordD3*);
    void checkClosedness() const;
};

#endif /* POLYHEDRON_H_ */
