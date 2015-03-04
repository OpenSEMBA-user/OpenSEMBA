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
	virtual ~Tet4();
	Tet4(
	 const CoordinateGroup<>&,
	 const uint vId[4],
	 const uint id_ = 0,
     const uint matId_ = 0,
     const uint layerId_ = 0);
	Tet4(
	 const CoordD3* v_[4],
	 const uint id_ = 0,
     const uint matId_ = 0,
     const uint layerId_ = 0);
	Tet4&
	 operator=(const Tet4& rhs);
	bool
	 isQuadratic() const {return false;}
	uint
	 numberOfCoordinates() const {return 4;}
	uint
	 numberOfSideCoordinates(const uint f = 0) const {return 3;}
	const CoordD3*
	 getV(const uint i) const {return v[i];}
	const CoordD3*
	 getSideV(const uint f, const uint i) const;
	void
	 setV(const uint i, const CoordD3*);
	const CoordD3*
	 getVertex(const uint i) const {return v[tet.vertex(i)];}
	const CoordD3*
	 getSideVertex(const uint f, const uint i) const;
	bool
	 isCurved() const;
	bool
	 isCurvedFace(const uint face) const;
	double
	 getAreaOfFace(const uint face) const;
	double
	 getVolume() const;
	const Simplex&
	 getTet() const {return tet;}
	void
	 check() const;
	void
	 printInfo() const;
	bool
	 isFaceContainedInPlane(
	  const uint face,
	  const CartesianPlane plane) const;
    bool
     isInnerPoint(const CVecD3& pos) const;
private:
	static const SimplexTet<1> tet;
	const CoordD3* v[4];
	bool
	 hasZeroVolume() const;
};
const SimplexTet<1> Tet4::tet;

#endif /* TET4_H_ */
