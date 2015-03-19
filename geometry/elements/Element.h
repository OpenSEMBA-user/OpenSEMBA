#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <iostream>
#include <limits>
#include <vector>

using namespace std;

#include "../math/MathMatrix.h"
#include "../math/CartesianVector.h"
#include "CoordinateGroup.h"
#include "Box.h"
#include "Grid.h"
#include "IdBase.h"
#include "Layer.h"

#include "ClassIdBase.h"

CreateId(ElementId);
CreateId(MatId);

template<class T = void>
class Element;

template<>
class Element<void> : public ClassIdBase<ElementId> {
public:
    Element();
    Element(const ElementId id,
            const LayerId layId = LayerId(0),
            const MatId   matId = MatId(0));
    Element(const Element& rhs);
    virtual ~Element();

    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }

    virtual UInt numberOfFaces      () const = 0;
    virtual UInt numberOfVertices   () const = 0;
    virtual UInt numberOfCoordinates() const = 0;

    virtual UInt numberOfSideVertices   (const UInt f = 0) const = 0;
    virtual UInt numberOfSideCoordinates(const UInt f = 0) const = 0;

    LayerId getLayerId() const { return layerId_; }
    MatId   getMatId  () const { return matId_;   }

    virtual void setLayerId(const LayerId layerId) { layerId_= layerId; }
    virtual void setMatId  (const MatId   matId  ) { matId_  = matId;   }

    virtual void printInfo() const = 0;

private:
    LayerId layerId_;
    MatId   matId_;
};

template<class T>
class Element : public virtual Element<void> {
public:
    Element();
    virtual ~Element();

    bool isCoordinate(const Coordinate<T,3>* coord) const;

    virtual bool isStructured(const Grid3&) const { return false; }
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

    virtual Element<Int>* toStructured(CoordinateGroup<CoordI3>&,
                                       const Grid3&) const;

    virtual void printInfo() const;

protected:
    void ascendingOrder(UInt nVal, UInt* val) const;
};

typedef Element<void> Elem;
typedef Element<Real> ElemR;
typedef Element<Int>  ElemI;

#endif
