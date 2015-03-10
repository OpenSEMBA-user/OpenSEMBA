/*
 * Tet.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET_H_
#define TET_H_

#include "Volume.h"
#include "Tri6.h"
#include "../math/SimplexTet.h"

class Tet : public Volume {
public:
    Tet(const LayerId layerId = LayerId(0),
        const MatId   matId   = MatId(0));
    Tet(const ElementId id,
        const LayerId layerId = LayerId(0),
        const MatId   matId   = MatId(0));
    Tet(const Tet& rhs);
    virtual ~Tet();

    virtual bool isCurvedFace(const UInt face) const = 0;
    virtual bool isFaceContainedInPlane(const UInt face,
                                        const CartesianPlane plane) const = 0;

    inline UInt numberOfFaces   () const { return 4; }
    inline UInt numberOfVertices() const { return 4; }

    inline UInt numberOfSideVertices(const UInt f = 0) const { return 3; }

    virtual const Simplex& getTet() const = 0;
    virtual Real getVolume() const = 0;
    virtual Real getAreaOfFace(const UInt face) const = 0;
    virtual Tri3* getTri3Face(const UInt f) const;
    virtual void getOrderedSideVerticesId(UInt val[3], UInt f) const;

    void getCubaturePositions(
        CVecR3 cubaturePositions[SimplexTet<1>::ncp]) const;
    void getCubatureJacobian(
        StaMatrix<Real,4,4> cJ[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianDeterminant(
        Real cJDet[SimplexTet<2>::ncp],
        const StaMatrix<Real, 4, 4> cJ[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianDeterminant(Real cJDet[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianHat(
        StaMatrix<Real,4,3> cJHat[SimplexTet<2>::ncp],
        const StaMatrix<Real,4,4> cJ[SimplexTet<2>::ncp],
        const Real cJDet[SimplexTet<2>::ncp]) const;
};

#endif /* TET_H_ */
