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
