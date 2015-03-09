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

    virtual bool isCurvedFace(const uint face) const = 0;
    virtual bool isFaceContainedInPlane(const uint face,
                                        const CartesianPlane plane) const = 0;

    inline uint numberOfFaces   () const { return 4; }
    inline uint numberOfVertices() const { return 4; }

    inline uint numberOfSideVertices(const uint f = 0) const { return 3; }

    virtual const Simplex& getTet() const = 0;
    virtual double getVolume() const = 0;
    virtual double getAreaOfFace(const uint face) const = 0;
    virtual Tri3 getTri3Face(const uint f) const;
    virtual void getOrderedSideVerticesId(uint val[3], uint f) const;

    void getCubaturePositions(
        CVecD3 cubaturePositions[SimplexTet<1>::ncp]) const;
    void getCubatureJacobian(
        StaMatrix<double,4,4> cJ[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianDeterminant(
        double cJDet[SimplexTet<2>::ncp],
        const StaMatrix<double, 4, 4> cJ[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianDeterminant(double cJDet[SimplexTet<2>::ncp]) const;
    void getCubatureJacobianHat(
        StaMatrix<double,4,3> cJHat[SimplexTet<2>::ncp],
        const StaMatrix<double,4,4> cJ[SimplexTet<2>::ncp],
        const double cJDet[SimplexTet<2>::ncp]) const;
};

#endif /* TET_H_ */
