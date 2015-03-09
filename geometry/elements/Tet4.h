/*
 * Tet4.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET4_H_
#define TET4_H_

#include "Tet.h"

class Tet4 : public Tet {
public:
    Tet4();
    Tet4(const CoordinateGroup<>&,
         const ElementId id,
         const CoordinateId vId[4],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tet4(const ElementId id,
         const CoordD3* v[4],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tet4(const Tet4& rhs);
    virtual ~Tet4();

    ClassBase* clone() const;

    bool isInnerPoint(const CVecD3& pos) const;
    bool isCurvedFace(const uint face) const;
    bool isFaceContainedInPlane(const uint face,
                                const CartesianPlane plane) const;

	uint numberOfCoordinates() const { return 4; }

	uint numberOfSideCoordinates(const uint f = 0) const { return 3; }

	const CoordD3* getV(const uint i) const { return v_[i]; }
	const CoordD3* getSideV(const uint f, const uint i) const;

	const CoordD3* getVertex(const uint i) const { return v_[tet.vertex(i)]; }
	const CoordD3* getSideVertex(const uint f, const uint i) const;

    double getVolume() const;
    const Simplex& getTet() const { return tet; }
	double getAreaOfFace(const uint face) const;

    void setV(const uint i, const CoordD3*);

    void printInfo() const;
    void check() const;
private:
	static const SimplexTet<1> tet;
	const CoordD3* v_[4];

	bool hasZeroVolume() const;
};
const SimplexTet<1> Tet4::tet;

#endif /* TET4_H_ */
