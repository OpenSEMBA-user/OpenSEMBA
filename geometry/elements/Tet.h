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
	Tet();
	Tet(const uint id_, const uint matId_, const uint layerId_);
	virtual ~Tet();
	inline unsigned int
	 numberOfFaces() const {return 4;}
	inline unsigned int
	 numberOfVertices() const {return 4;}
	inline unsigned int
	 numberOfSideVertices(const uint f = 0) const {return 3;}
	virtual bool
	 isCurvedFace(const unsigned int face) const = 0;
	virtual void
	 getOrderedSideVerticesId(
	  unsigned int val[3],
	  unsigned int f) const;
	virtual double
	 getAreaOfFace(const unsigned int face) const = 0;
	virtual double
	 getVolume() const = 0;
	BoxD3
	 getBoundOfFace(
	  const unsigned int face) const;
	virtual const Simplex&
	 getTet() const = 0;
	virtual Tri3
	 getTri3Face(const unsigned int f) const;
	bool
	 isTet() const {return true;}
	void
	 getCubaturePositions(
	  CVecD3 cubaturePositions[SimplexTet<1>::ncp]) const;
	void
	 getCubatureJacobian(
	  StaMatrix<double,4,4> cJ[SimplexTet<2>::ncp]) const;
	void
	 getCubatureJacobianDeterminant(
	  double cJDet[SimplexTet<2>::ncp],
	  const StaMatrix<double, 4, 4> cJ[SimplexTet<2>::ncp]) const;
	void
	 getCubatureJacobianDeterminant(
	  double cJDet[SimplexTet<2>::ncp]) const;
	void
	 getCubatureJacobianHat(
	  StaMatrix<double,4,3> cJHat[SimplexTet<2>::ncp],
	  const StaMatrix<double,4,4> cJ[SimplexTet<2>::ncp],
	  const double cJDet[SimplexTet<2>::ncp]) const;
	virtual bool
	 isFaceContainedInPlane(
	  const unsigned int face,
	  const CartesianPlane plane) const = 0;
};

#endif /* TET_H_ */
