/*
 * Tet10.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET10_H_
#define TET10_H_

#include "Tet.h"
#include "Tet4.h"

class Tet10 : public Tet {
public:
	Tet10(
	 const CoordinateGroup&,
     const unsigned int id_,
     const unsigned int matId_,
	 const unsigned int vId[10]);
	~Tet10();
	Tet10&
	 operator=(const Tet10& rhs);
    bool
	 isQuadratic() const {return true;}
	bool
	 isCurved() const;
	bool
	 isCurvedFace(const unsigned int face) const;
	Tri6
	 getTri6Face(const unsigned int f) const;
	unsigned int
	 numberOfCoordinates() const {return 10;}
	unsigned int
	 numberOfSideCoordinates(const uint f = 0) const {return 6;}
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getSideV(const unsigned int f, const unsigned int i) const;
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int f, const unsigned int i) const;
	double
	 getAreaOfFace(const unsigned int face) const;
	double
	 getVolume() const;
	Tet4
	 linearize() const;
	const Simplex&
	 getTet() const {return tet;}
	bool
	 isFaceContainedInPlane(
	  const unsigned int face,
	  const CartesianPlane plane) const;
	void
	 printInfo() const;
	void
	 check() const;
private:
	static const SimplexTri<2> tri;
	static const SimplexTet<2> tet;
	const Coordinate<double,3>* v[10];
};
const SimplexTri<2> Tet10::tri;
const SimplexTet<2> Tet10::tet;


#endif /* TET10_H_ */
