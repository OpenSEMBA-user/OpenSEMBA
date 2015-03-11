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

template<class T>
class Element : public ElementBase {
public:
    Element(const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Element(const ElementId id,
            const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Element(const Element<T>& rhs);
    virtual ~Element();

    bool isCoordinate(const Coordinate<T,3>* coord) const;

    virtual bool isInnerPoint(const CartesianVector<T,3>& pos) const;

    virtual const Coordinate<T,3>* getV    (const UInt i) const = 0;
    virtual const Coordinate<T,3>* getSideV(const UInt f,
                                            const UInt i) const = 0;

    virtual const Coordinate<T,3>* getVertex    (const UInt i) const = 0;
    virtual const Coordinate<T,3>* getSideVertex(const UInt f,
                                                 const UInt i) const = 0;

    Box<T,3> getBound() const;
    // Returns ptr to coord with min(max) lexicographical position.
    virtual const Coordinate<T,3>* getMinV() const;
    virtual const Coordinate<T,3>* getMaxV() const;

    virtual void setV(const UInt i, const Coordinate<T,3>*);

    virtual void printInfo() const;

protected:
    static const Real tolerance;

    void ascendingOrder(UInt nVal, UInt* val) const;
};

#include "Element.hpp"

typedef Element<Real> ElemR;
typedef Element<Int>  ElemI;

#endif
