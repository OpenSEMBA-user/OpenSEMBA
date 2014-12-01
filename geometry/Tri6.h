#ifndef TRI6_H_
#define TRI6_H_

#include "Tri.h"
#include "Tri3.h"

class Tri6 : public Tri {
public:
	Tri6();
	Tri6(
	 const CoordinateGroup&,
     const unsigned int id_,
     const unsigned int mattiId_,
	 const unsigned int vId[6],
	 const CartesianVector<double,3>& normal_);
	Tri6(
	 const Coordinate<double,3>*[6]);
	bool
	 isQuadratic() const {return true;}
	bool
	 isCurved() const;
	unsigned int
	 numberOfCoordinates() const {return 6;}
	unsigned int
	 numberOfSideCoordinates(const uint f = 0) const {return 3;}
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	const Coordinate<double,3>*
	 getSideV(const unsigned int face, const unsigned int i) const;
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int face, const unsigned int i) const;
	double
	 getArea() const;
	Tri3
	 linearize() const;
	void
	 printInfo() const;
	void
	 getCubatureDifferentials(
	  double csdf[SimplexTri<2>::ncp]) const;
	void
	 getCubatureNormals(
	  CartesianVector<double,3> cn[SimplexTri<2>::ncp]) const;
	void
	 getCubatureNodes(
	  CartesianVector<double,3> nodes[SimplexTri<2>::ncp]) const;
protected:
	static const SimplexTri<2> geo;
	const Coordinate<double,3>* v[6];
	void
	 getCubatureTangentsVecProds(
	  CartesianVector<double,3> csTanVecProd[SimplexTri<2>::ncp]) const;
};

const SimplexTri<2> Tri6::geo;

#endif
