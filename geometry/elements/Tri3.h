#ifndef TRI3_H_
#define TRI3_H_

#include "Tri.h"

class Tri3 : public Tri {
public:
    Tri3();
	Tri3(const CoordinateGroup<>&,
         const ElementId id,
         const CoordinateId vId[3],
         const LayerId layerId = LayerId(0),
         const uint matId   = 0);
	Tri3(const ElementId id,
         const CoordD3* v[3],
         const LayerId layerId = LayerId(0),
         const uint matId   = 0);
    Tri3(const Tri3& rhs);
    Tri3(const ElementId id, const Tri3& rhs);
	virtual ~Tri3();
    
    ClassBase* clone()                   const;
    ClassBase* clone(const ElementId id) const;
    
	uint numberOfCoordinates() const { return 3; }
    
	uint numberOfSideCoordinates(const uint f = 0) const { return 2; }
    
	const CoordD3* getV     (const uint i) const {return v_[i];}
	const CoordD3* getVertex(const uint i) const;
    
	const CoordD3* getSideV     (const uint face, const uint i) const;
	const CoordD3* getSideVertex(const uint face, const uint i) const;
    
	double getArea() const;
    
	void getCubatureDifferentials(double  csdf[SimplexTri<1>::ncp]) const;
	void getCubatureNormals      (CVecD3    cn[SimplexTri<1>::ncp]) const;
	void getCubatureNodes        (CVecD3 nodes[SimplexTri<1>::ncp]) const;
    
	void setV(const uint i, const CoordD3*);
    
	Tri3 linearize() const;
    
	void printInfo() const;
	
    void check() const;

protected:
	static const SimplexTri<1> geo;
	const CoordD3* v_[3];
	
    void getCubatureTangentsVecProds(
        CVecD3 csTanVecProd[SimplexTri<1>::ncp]) const;
};
const SimplexTri<1> Tri3::geo;

#endif
