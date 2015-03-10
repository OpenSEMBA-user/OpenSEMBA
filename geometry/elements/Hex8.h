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
         const CVecR3& min,
         const CVecR3& max,
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Hex8(const Hex8& rhs);
    virtual ~Hex8();

    ClassBase* clone() const;

    bool isRegular() const;
    inline bool isQuadratic() const { return false; }
    inline bool isCurvedFace(const UInt f) const { return false; }

    inline UInt numberOfFaces      () const { return 6; }
    inline UInt numberOfVertices   () const { return 8; }
    inline UInt numberOfCoordinates() const { return 8; }

    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 4; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 4; }

    const CoordR3* getV    (const UInt i) const { return v_[i]; }
    const CoordR3* getSideV(const UInt f, const UInt i) const;

    const CoordR3* getVertex    (const UInt i) const { return v_[i]; }
    const CoordR3* getSideVertex(const UInt f, const UInt i) const;

    Real getAreaOfFace(const UInt face) const;

    void setV(const UInt i, const CoordR3*);

    void printInfo() const;
private:
    const CoordR3* v_[8];

    const static Real tolerance;
};

#endif /* HEX8_H_ */
