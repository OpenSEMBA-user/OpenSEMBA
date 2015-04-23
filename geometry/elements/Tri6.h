#ifndef TRI6_H_
#define TRI6_H_

#include "Tri.h"
#include "Tri3.h"

class Tri6 : public Tri {
public:
    Tri6();
    Tri6(const CoordinateGroup<CoordR3>&,
         const ElementId id,
         const CoordinateId vId[6],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tri6(const ElementId id,
         const CoordR3*[6],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Tri6(const Tri6& rhs);
    virtual ~Tri6();

    DEFINE_CLONE(Tri6);

    bool isCurved   () const;
    bool isQuadratic() const { return true; }

    UInt numberOfCoordinates() const { return 6; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 3; }

    const CoordR3* getV(const UInt i) const {return v_[i];}
    const CoordR3* getSideV(const UInt face, const UInt i) const;

    const CoordR3* getVertex(const UInt i) const;
    const CoordR3* getSideVertex(const UInt face, const UInt i) const;

    Real getArea() const;

    void getCubatureDifferentials(Real  csdf[SimplexTri<2>::ncp]) const;
    void getCubatureNormals      (CVecR3    cn[SimplexTri<2>::ncp]) const;
    void getCubatureNodes        (CVecR3 nodes[SimplexTri<2>::ncp]) const;

    void setV(const UInt i, const CoordR3*);

    Tri3* linearize() const;

    void printInfo() const;

protected:
    static const SimplexTri<2> geo;
    const CoordR3* v_[6];
    void getCubatureTangentsVecProds(
        CVecR3 csTanVecProd[SimplexTri<2>::ncp]) const;
};

const SimplexTri<2> Tri6::geo;

#endif
