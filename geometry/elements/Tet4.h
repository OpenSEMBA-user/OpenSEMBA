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
    Tet4(const CoordinateGroup<CoordR3>&,
         const ElementId id,
         const CoordinateId vId[4],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tet4(const ElementId id,
         const CoordR3* v[4],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tet4(const Tet4& rhs);
    virtual ~Tet4();

    DEFINE_CLONE(Tet4);

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
const SimplexTet<1> Tet4::tet;

#endif /* TET4_H_ */
