/*
 * Hex8.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef HEX8_H_
#define HEX8_H_

#include "Volume.h"

template<class T = void>
class Hex8;

template<>
class Hex8<void> : public virtual Vol {
public:
    Hex8() {}
    virtual ~Hex8() {}

    inline bool isQuadratic() const { return false; }

    inline UInt numberOfFaces      () const { return 6; }
    inline UInt numberOfVertices   () const { return 8; }
    inline UInt numberOfCoordinates() const { return 8; }

    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 4; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 4; }
};

template<class T>
class Hex8 : public virtual Volume<T>,
             public virtual Hex8<void> {
public:
    Hex8();
    Hex8(const CoordinateGroup<Coordinate<T,3> >&,
         const ElementId id,
         const CoordinateId vId[8],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Hex8(CoordinateGroup<Coordinate<T,3> >&,
         const ElementId id,
         const Box<T,3>& box,
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Hex8(const Hex8<T>& rhs);
    virtual ~Hex8();

    ClassBase* clone() const;

    bool isStructured(const Grid3&) const;

    bool isRegular() const;
    inline bool isCurvedFace(const UInt f) const { return false; }

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f, const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideVertex(const UInt f, const UInt i) const;

    Real getAreaOfFace(const UInt face) const;

    void setV(const UInt i, const Coordinate<T,3>*);

    ElemI* toStructured(CoordinateGroup<CoordI3>&, const Grid3&) const;

    void printInfo() const;
private:
    const Coordinate<T,3>* v_[8];

    const static Real tolerance;
};

typedef Hex8<Real> HexR8;
typedef Hex8<Int > HexI8;

#endif /* HEX8_H_ */
