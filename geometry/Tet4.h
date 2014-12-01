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
	 const CoordinateGroup&,
	 const unsigned int id_,
	 const unsigned int matId_,
	 const unsigned int vId[4]);
	Tet4(
	 const unsigned int id_,
	 const unsigned int matId_,
	 const Coordinate<double,3>* v_[4]);
	Tet4&
	 operator=(const Tet4& rhs);
	bool
	 isQuadratic() const {return false;}
	unsigned int
	 numberOfCoordinates() const {return 4;}
	unsigned int
	 numberOfSideCoordinates(const uint f = 0) const {return 3;}
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	const Coordinate<double,3>*
	 getSideV(const unsigned int f, const unsigned int i) const;
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const {return v[tet.vertex(i)];}
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int f, const unsigned int i) const;
	Tri3
	 getTri3Face(const unsigned int f) const;
	bool
	 isCurved() const;
	bool
	 isCurvedFace(const unsigned int face) const;
	double
	 getAreaOfFace(const unsigned int face) const;
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
	  const unsigned int face,
	  const CartesianPlane plane) const;
private:
	static const SimplexTet<1> tet;
	const Coordinate<double,3>* v[4];
	bool
	 hasZeroVolume() const;
};
const SimplexTet<1> Tet4::tet;

#endif /* TET4_H_ */
