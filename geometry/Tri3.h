#ifndef TRI3_H_
#define TRI3_H_

#include "Tri.h"

class Tri3 : public Tri {
public:
	Tri3();
	virtual ~Tri3();
	Tri3(
	 const CoordinateGroup&,
     const uint vId[3],
	 const CVecD3& normal_,
	 const uint id_ = 0,
     const uint matId_ = 0,
     const uint layerId_ = 0);
	Tri3(
	 const CoordD3* v_[3],
	 const CVecD3 normal_ = CVecD3(),
	 const uint id = 0,
	 const uint matId = 0,
	 const uint layerId = 0);
	Tri3&
	 operator=(const Tri3& rhs);
	bool
	 isQuadratic() const {return false;}
	bool
	 isCurved() const {return false;}
	uint
	 numberOfCoordinates() const {return 3;}
	uint
	 numberOfSideCoordinates(const uint f = 0) const {return 2;}
	const CoordD3*
	 getV(const uint i) const {return v[i];}
	void
	 setV(const uint i, const CoordD3*);
	const CoordD3*
	 getVertex(const uint i) const;
	const CoordD3*
	 getSideV(const uint face, const uint i) const;
	const CoordD3*
	 getSideVertex(const uint face, const uint i) const;
	double
	 getArea() const;
	void
	 getCubatureDifferentials(
	  double csdf[SimplexTri<1>::ncp]) const;
	void
	 getCubatureNormals(
	  CVecD3 cn[SimplexTri<1>::ncp]) const;
	void
	 getCubatureNodes(
	  CVecD3 nodes[SimplexTri<1>::ncp]) const;
	Tri3
	 linearize() const;
	void
	 printInfo() const;
	void
	 check() const;
protected:
	static const SimplexTri<1> geo;
	const CoordD3* v[3];
	void
	 getCubatureTangentsVecProds(
	  CVecD3 csTanVecProd[SimplexTri<1>::ncp]) const;
};
const SimplexTri<1> Tri3::geo;

#endif
