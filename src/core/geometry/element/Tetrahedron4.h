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
 * Tet4.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET4_H_
#define TET4_H_

#include <geometry/elements/Tetrahedron.h>

class Tetrahedron4 : public Tetrahedron {
public:
    Tetrahedron4();
    Tetrahedron4(const GroupCoordinates<CoordR3>&,
         const ElementId id,
         const CoordinateId vId[4],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tetrahedron4(const ElementId id,
         const CoordR3* v[4],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tetrahedron4(const Tetrahedron4& rhs);
    virtual ~Tetrahedron4();

    DEFINE_CLONE(Tetrahedron4);

    bool isInnerPoint(const CVecR3& pos) const;
    bool isCurvedFace(const UInt face) const;
    bool isFaceContainedInPlane(const UInt face,
                                const CartesianPlane plane) const;

	UInt numberOfCoordinates() const { return 4; }

	UInt numberOfSideCoordinates(const UInt f = 0) const { return 3; }

	const CoordR3* getV(const UInt i) const { return v_[i]; }
	const CoordR3* getSideV(const UInt f, const UInt i) const;

	const CoordR3* getVertex(const UInt i) const { return v_[tet.vertex(i)]; }
	const CoordR3* getSideVertex(const UInt f, const UInt i) const;

    Real getVolume() const;
    const Simplex& getTet() const { return tet; }
	Real getAreaOfFace(const UInt face) const;

    void setV(const UInt i, const CoordR3*);

    void printInfo() const;
    void check() const;

private:
	static const SimplexTet<1> tet;
	const CoordR3* v_[4];

	bool hasZeroVolume() const;
};
const SimplexTet<1> Tetrahedron4::tet;

typedef Tetrahedron4 Tet4;

#endif /* TET4_H_ */
