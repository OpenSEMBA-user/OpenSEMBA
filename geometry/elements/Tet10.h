/*
 * Tet10.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET10_H_
#define TET10_H_

#include "Tet.h"
#include "Tet4.h"

class Tet10 : public Tet {
public:
    Tet10();
    Tet10(const CoordinateGroup<>&,
          const ElementId id,
          const CoordinateId vId[10],
          const uint layerId = 0,
          const uint matId   = 0);
    Tet10(const Tet10& rhs);
    Tet10(const ElementId id, const Tet10& rhs);
    virtual ~Tet10();

    ClassBase* clone()                   const;
    ClassBase* clone(const ElementId id) const;

    bool isCurved() const;
    bool isQuadratic() const {return true;}
    bool isCurvedFace(const uint f) const;
    bool isFaceContainedInPlane(const uint face,
                                const CartesianPlane plane) const;

    uint numberOfCoordinates() const { return 10; }

    uint numberOfSideCoordinates(const uint f = 0) const { return 6; }

    const CoordD3* getV(const uint i) const {return v_[i];}
    const CoordD3* getSideV(const uint f, const uint i) const;

    const CoordD3* getVertex(const uint i) const;
    const CoordD3* getSideVertex(const uint f, const uint i) const;

    double getVolume() const;
    const Simplex& getTet() const {return tet;}
    double getAreaOfFace(const uint face) const;
    Tri6 getTri6Face(const uint f) const;

    void setV(const uint i, const CoordD3*);

    Tet4 linearize() const;

    void printInfo() const;
    void check() const;
private:
    static const SimplexTri<2> tri;
    static const SimplexTet<2> tet;
    const CoordD3* v_[10];
};
const SimplexTri<2> Tet10::tri;
const SimplexTet<2> Tet10::tet;


#endif /* TET10_H_ */
