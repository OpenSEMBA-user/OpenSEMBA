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

class Polyhedron : public Volume<Real> {
public:
    Polyhedron();
    Polyhedron(const ElementId id,
               const vector<const Polygon*>& faces,
               const LayerId layerId = LayerId(0),
               const MatId   matId   = MatId(0));
    Polyhedron(const Polyhedron& rhs);
    virtual ~Polyhedron();

    ClassBase* clone() const;

    bool isCurvedFace(const UInt f) const;

    UInt numberOfFaces      () const;
    UInt numberOfVertices   () const;
    UInt numberOfCoordinates() const;

    UInt numberOfSideVertices   (const UInt f = 0) const;
    UInt numberOfSideCoordinates(const UInt f = 0) const;
    const CoordR3* getV(const UInt i) const;
    const CoordR3* getSideV(const UInt f, const UInt i) const;
    const CoordR3* getVertex(const UInt i) const;
    const CoordR3* getSideVertex(const UInt f, const UInt i) const;

    const Polygon* getFace(const UInt f) const;
    Real getAreaOfFace(const UInt f) const;

    void printInfo() const;

private:
    vector<const CoordR3*> v_;
    vector<const Polygon*> face_;

    void addV(const CoordR3*);
    void checkClosedness() const;
};

#endif /* POLYHEDRON_H_ */
