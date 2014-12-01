#ifndef TRI3_H_
#define TRI3_H_

#include "Tri.h"

class Tri3 : public Tri {
public:
	Tri3();
	virtual ~Tri3();
	Tri3(
	 const CoordinateGroup&,
     const unsigned int id_,
     const unsigned int matId_,
	 const unsigned int vId[3],
	 const CartesianVector<double,3>& normal_);
	Tri3(
	 const unsigned int id_,
	 const unsigned int matId_,
	 const Coordinate<double,3>* v_[3],
	 const CartesianVector<double,3>& normal_);
	Tri3(
	 const Coordinate<double,3>* v_[3],
	 const uint matId = 0);
	Tri3&
	 operator=(const Tri3& rhs);
	bool
	 isQuadratic() const {return false;}
	bool
	 isCurved() const {return false;}
	unsigned int
	 numberOfCoordinates() const {return 3;}
	unsigned int
	 numberOfSideCoordinates(const uint f = 0) const {return 2;}
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideV(const unsigned int face, const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int face, const unsigned int i) const;
	double
	 getArea() const;
	void
	 getCubatureDifferentials(
	  double csdf[SimplexTri<1>::ncp]) const;
	void
	 getCubatureNormals(
	  CartesianVector<double,3> cn[SimplexTri<1>::ncp]) const;
	void
	 getCubatureNodes(
	  CartesianVector<double,3> nodes[SimplexTri<1>::ncp]) const;
	Tri3
	 linearize() const;
	void
	 printInfo() const;
	void
	 check() const;
protected:
	static const SimplexTri<1> geo;
	const Coordinate<double,3>* v[3];
	void
	 getCubatureTangentsVecProds(
	  CartesianVector<double,3> csTanVecProd[SimplexTri<1>::ncp]) const;
};
const SimplexTri<1> Tri3::geo;

#endif
