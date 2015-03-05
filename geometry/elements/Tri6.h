#ifndef TRI6_H_
#define TRI6_H_

#include "Tri.h"
#include "Tri3.h"

class Tri6 : public Tri {
public:
    Tri6();
    Tri6(const CoordinateGroup<>&,
         const ElementId id,
         const CoordinateId vId[6],
         const LayerId layerId = LayerId(0),
         const uint matId   = 0);
    Tri6(const ElementId id,
         const CoordD3*[6],
         const LayerId layerId = LayerId(0),
         const uint matId   = 0);
    Tri6(const Tri6& rhs);
    Tri6(const ElementId id_, const Tri6& rhs);
    virtual ~Tri6();

    ClassBase* clone()                   const;
    ClassBase* clone(const ElementId id) const;

    bool isCurved   () const;
    bool isQuadratic() const { return true; }

    uint numberOfCoordinates() const { return 6; }

    uint numberOfSideCoordinates(const uint f = 0) const { return 3; }

    const CoordD3* getV(const uint i) const {return v_[i];}
    const CoordD3* getSideV(const uint face, const uint i) const;

    const CoordD3* getVertex(const uint i) const;
    const CoordD3* getSideVertex(const uint face, const uint i) const;

    double getArea() const;

    void getCubatureDifferentials(double  csdf[SimplexTri<2>::ncp]) const;
    void getCubatureNormals      (CVecD3    cn[SimplexTri<2>::ncp]) const;
    void getCubatureNodes        (CVecD3 nodes[SimplexTri<2>::ncp]) const;

    void setV(const uint i, const CoordD3*);

    Tri3 linearize() const;

    void printInfo() const;

protected:
    static const SimplexTri<2> geo;
    const CoordD3* v_[6];
    void getCubatureTangentsVecProds(
        CVecD3 csTanVecProd[SimplexTri<2>::ncp]) const;
};

const SimplexTri<2> Tri6::geo;

#endif
