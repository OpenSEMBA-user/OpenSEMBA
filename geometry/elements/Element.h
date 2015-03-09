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
            const uint matId   = 0);
    Element(const ElementId id,
            const LayerId layerId = LayerId(0),
            const uint matId   = 0);
    Element(const Element& rhs);
    virtual ~Element();

    bool isCoordinate(const CoordD3* coord) const;

    virtual bool isInnerPoint(const CVecD3& pos) const;

    virtual const CoordD3* getV    (const uint i) const = 0;
    virtual const CoordD3* getSideV(const uint f,
            const uint i) const = 0;

    virtual const CoordD3* getVertex    (const uint i) const = 0;
    virtual const CoordD3* getSideVertex(const uint f,
            const uint i) const = 0;

    BoxD3 getBound() const;
    // Returns ptr to coord with min(max) lexicographical position.
    virtual const CoordD3* getMinV() const;
    virtual const CoordD3* getMaxV() const;

    virtual void setV(const uint i, const CoordD3*);

    virtual void printInfo() const;

protected:
    static const double tolerance;

    void ascendingOrder(uint nVal, uint* val) const;
};

#endif
