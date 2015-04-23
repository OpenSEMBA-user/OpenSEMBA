#ifndef TRI3_H_
#define TRI3_H_

#include "Tri.h"

class Tri3 : public Tri {
public:
    Tri3();
    Tri3(const CoordinateGroup<CoordR3>&,
         const ElementId id,
         const CoordinateId vId[3],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tri3(const ElementId id,
         const CoordR3* v[3],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tri3(const Tri3& rhs);
    virtual ~Tri3();

    DEFINE_CLONE(Tri3);

    UInt numberOfCoordinates() const { return 3; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 2; }

    const CoordR3* getV     (const UInt i) const {return v_[i];}
    const CoordR3* getVertex(const UInt i) const;

    const CoordR3* getSideV     (const UInt face, const UInt i) const;
    const CoordR3* getSideVertex(const UInt face, const UInt i) const;

    Real getArea() const;

    void getCubatureDifferentials(Real  csdf[SimplexTri<1>::ncp]) const;
    void getCubatureNormals      (CVecR3    cn[SimplexTri<1>::ncp]) const;
    void getCubatureNodes        (CVecR3 nodes[SimplexTri<1>::ncp]) const;

    void setV(const UInt i, const CoordR3*);

    Tri3* linearize() const;

    void printInfo() const;
    void check() const;

protected:
    static const SimplexTri<1> geo;
    const CoordR3* v_[3];

    void getCubatureTangentsVecProds(
    CVecR3 csTanVecProd[SimplexTri<1>::ncp]) const;
};
const SimplexTri<1> Tri3::geo;

#endif
