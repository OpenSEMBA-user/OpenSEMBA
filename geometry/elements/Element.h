#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <iostream>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

#include "../math/MathMatrix.h"
#include "../math/CartesianVector.h"
#include "coordinates/CoordinateGroup.h"
#include "Box.h"
#include "Grid.h"
#include "../../base/class/IdBase.h"
#include "../layers/Layer.h"

#include "../../base/class/ClassIdBase.h"
#include "../../base/class/ClassCompBase.h"

CREATE_ID(ElementId);
CREATE_ID(MatId);

class ElementBase : public virtual ClassIdBase<ElementId>,
                    public virtual ClassCompBase {
public:
    ElementBase(const LayerId layId = LayerId(0),
                const MatId   matId = MatId(0));
    ElementBase(const ElementBase& rhs);
    virtual ~ElementBase();

    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }

    virtual UInt numberOfFaces      () const = 0;
    virtual UInt numberOfVertices   () const = 0;
    virtual UInt numberOfCoordinates() const = 0;

    virtual UInt numberOfSideVertices   (const UInt f = 0) const = 0;
    virtual UInt numberOfSideCoordinates(const UInt f = 0) const = 0;

    LayerId getLayerId() const { return layId_; }
    MatId   getMatId  () const { return matId_;   }

    virtual void setLayerId(const LayerId layId) { layId_ = layId; }
    virtual void setMatId  (const MatId   matId) { matId_ = matId; }

    virtual void printInfo() const = 0;
private:
    LayerId layId_;
    MatId   matId_;
};

template<class T>
class Element : public virtual ElementBase {
public:
    Element();
    virtual ~Element();

    bool operator<(const ClassCompBase& rhs) const;

    bool isCoordinate(const Coordinate<T,3>* coord) const;

    virtual bool isStructured(const Grid3&,
                              const Real = Grid3::tolerance) const;
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
                                       const Grid3&,
                                       const Real = Grid3::tolerance) const;
    virtual Element<Real>* toUnstructured(CoordinateGroup<CoordR3>&,
                                          const Grid3&) const;

protected:
    void ascendingOrder(UInt nVal, UInt* val) const;

    bool vertexInCell (const Grid3& grid, const Real tol) const;
    bool vertexInBound() const;
    CoordinateId* vertexToStructured(CoordinateGroup<CoordI3>& cG,
                                     const Grid3& grid,
                                     const Real tol) const;
    CoordinateId* vertexToUnstructured(CoordinateGroup<CoordR3>& cG,
                                       const Grid3& grid) const;

};

typedef ElementBase   Elem;
typedef Element<Real> ElemR;
typedef Element<Int>  ElemI;

#endif
