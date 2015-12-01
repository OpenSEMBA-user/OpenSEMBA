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
#ifndef TRI3_H_
#define TRI3_H_

#include <geometry/elements/Triangle.h>

class Triangle3 : public Triangle {
public:
    Triangle3();
    Triangle3(const GroupCoordinates<CoordR3>&,
         const ElementId id,
         const CoordinateId vId[3],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Triangle3(const ElementId id,
         const CoordR3* v[3],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Triangle3(const Triangle3& rhs);
    virtual ~Triangle3();

    DEFINE_CLONE(Triangle3);

    UInt numberOfCoordinates() const { return 3; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 2; }

    const CoordR3* getV     (const UInt i) const {return v_[i];}
    const CoordR3* getVertex(const UInt i) const;

    const CoordR3* getSideV     (const UInt face, const UInt i) const;
    const CoordR3* getSideVertex(const UInt face, const UInt i) const;

    Real getArea() const;

    void getCubatureDifferentials(Real  csdf[SimplexTri<1>::ncp]) const;
    void getCubatureNormals      (CVecR3    cn[SimplexTri<1>::ncp]) const;
    void getCubatureNodes        (CVecR3 nodes[SimplexTri<1>::ncp]) const;

    void setV(const UInt i, const CoordR3*);

    Triangle3* linearize() const;

    void printInfo() const;
    void check() const;

protected:
    static const SimplexTri<1> geo;
    const CoordR3* v_[3];

    void getCubatureTangentsVecProds(
    CVecR3 csTanVecProd[SimplexTri<1>::ncp]) const;
};
const SimplexTri<1> Triangle3::geo;

typedef Triangle3 Tri3;

#endif
