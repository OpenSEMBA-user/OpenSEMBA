/*
 * Tet.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET_H_
#define TET_H_

#include <geometry/elements/Triangle6.h>
#include "math/SimplexTet.h"
#include "Volume.h"

class Tetrahedron : public Volume<Real> {
public:
    Tetrahedron();
    virtual ~Tetrahedron();

    virtual bool isCurvedFace(const UInt face) const = 0;
    virtual bool isFaceContainedInPlane(const UInt face,
                                        const CartesianPlane plane) const = 0;

    inline UInt numberOfFaces   () const { return 4; }
    inline UInt numberOfVertices() const { return 4; }
    inline UInt numberOfSideVertices(const UInt f = 0) const { return 3; }
    virtual const Simplex& getTet() const = 0;
    virtual Real getVolume() const = 0;
    virtual Real getAreaOfFace(const UInt face) const = 0;
    virtual Triangle3* getTri3Face(const UInt f) const;
    void getCubaturePositions(
        CVecR3 cubaturePositions[SimplexTet<1>::ncp]) const;
    void getCubatureJacobian(
        MatR44 cJ[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianDeterminant(
        Real cJDet[SimplexTet<2>::ncp],
        const MatR44 cJ[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianDeterminant(Real cJDet[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianHat(
        MatR43 cJHat[SimplexTet<2>::ncp],
        const MatR44 cJ[SimplexTet<2>::ncp],
        const Real cJDet[SimplexTet<2>::ncp]) const;
};

typedef Tetrahedron Tet;

#endif /* TET_H_ */
