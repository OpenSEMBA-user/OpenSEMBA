/*
 * Tet10.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET10_H_
#define TET10_H_

#include <geometry/elements/Tetrahedron.h>
#include <geometry/elements/Tetrahedron4.h>

class Tetrahedron10 : public Tetrahedron {
public:
    Tetrahedron10();
    Tetrahedron10(const GroupCoordinates<CoordR3>&,
          const ElementId id,
          const CoordinateId vId[10],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
    Tetrahedron10(const Tetrahedron10& rhs);
    virtual ~Tetrahedron10();

    DEFINE_CLONE(Tetrahedron10);

    bool isCurved() const;
    bool isQuadratic() const {return true;}
    bool isCurvedFace(const UInt f) const;
    bool isFaceContainedInPlane(const UInt face,
                                const CartesianPlane plane) const;

    UInt numberOfCoordinates() const { return 10; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 6; }

    const CoordR3* getV(const UInt i) const {return v_[i];}
    const CoordR3* getSideV(const UInt f, const UInt i) const;

    const CoordR3* getVertex(const UInt i) const;
    const CoordR3* getSideVertex(const UInt f, const UInt i) const;

    Real getVolume() const;
    const Simplex& getTet() const {return tet;}
    Real getAreaOfFace(const UInt face) const;
    Triangle6 getTri6Face(const UInt f) const;

    void setV(const UInt i, const CoordR3*);

    Tetrahedron4* linearize() const;

    void printInfo() const;
    void check() const;

private:
    static const SimplexTri<2> tri;
    static const SimplexTet<2> tet;
    const CoordR3* v_[10];
};
const SimplexTri<2> Tetrahedron10::tri;
const SimplexTet<2> Tetrahedron10::tet;

typedef Tetrahedron10 Tet10;

#endif /* TET10_H_ */
