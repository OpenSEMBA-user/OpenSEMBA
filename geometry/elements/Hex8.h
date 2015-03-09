/*
 * Hex8.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef HEX8_H_
#define HEX8_H_

#include "Volume.h"

class Hex8 : public Volume {
public:
    Hex8();
    Hex8(const CoordinateGroup<>&,
         const ElementId id,
         const CoordinateId vId[8],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Hex8(const CoordinateGroup<>&,
         const ElementId id,
         const CVecD3& min,
         const CVecD3& max,
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Hex8(const Hex8& rhs);
    virtual ~Hex8();

    ClassBase* clone() const;

    bool        isRegular() const;
    inline bool isQuadratic() const { return false; }
    inline bool isCurvedFace(const uint f) const { return false; }

    inline uint numberOfFaces      () const { return 6; }
    inline uint numberOfVertices   () const { return 8; }
    inline uint numberOfCoordinates() const { return 8; }

    inline uint numberOfSideVertices   (const uint f = 0) const { return 4; }
    inline uint numberOfSideCoordinates(const uint f = 0) const { return 4; }

    const CoordD3* getV    (const uint i) const { return v_[i]; }
    const CoordD3* getSideV(const uint f, const uint i) const;

    const CoordD3* getVertex    (const uint i) const { return v_[i]; }
    const CoordD3* getSideVertex(const uint f, const uint i) const;

    double getAreaOfFace(const uint face) const;

    void setV(const uint i, const CoordD3*);

    void printInfo() const;
private:
    const CoordD3* v_[8];

    const static double tolerance;
};

#endif /* HEX8_H_ */
