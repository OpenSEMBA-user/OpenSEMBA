#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <limits>

#include "../math/MathMatrix.h"
#include "../math/CartesianVector.h"
#include "CoordinateGroup.h"
#include "Box.h"
#include <iostream>
#include <vector>

using namespace std;

#include "ElementBase.h"

class Element : public ElementBase {
public:
    Element(const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Element(const ElementId id,
            const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Element(const Element& rhs);
    virtual ~Element();

    bool isCoordinate(const CoordR3* coord) const;

    virtual bool isInnerPoint(const CVecR3& pos) const;

    virtual const CoordR3* getV    (const UInt i) const = 0;
    virtual const CoordR3* getSideV(const UInt f,
            const UInt i) const = 0;

    virtual const CoordR3* getVertex    (const UInt i) const = 0;
    virtual const CoordR3* getSideVertex(const UInt f,
            const UInt i) const = 0;

    BoxR3 getBound() const;
    // Returns ptr to coord with min(max) lexicographical position.
    virtual const CoordR3* getMinV() const;
    virtual const CoordR3* getMaxV() const;

    virtual void setV(const UInt i, const CoordR3*);

    virtual void printInfo() const;

protected:
    static const Real tolerance;

    void ascendingOrder(UInt nVal, UInt* val) const;
};

#endif
