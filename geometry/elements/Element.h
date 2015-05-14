#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

#include "base/error/Error.h"
#include "base/error/ErrorNotImplemented.h"
#include "math/MathMatrix.h"
#include "math/CartesianVector.h"
#include "geometry/Box.h"
#include "geometry/Grid.h"
#include "geometry/coordinates/GroupCoordinates.h"
#include "geometry/layers/Layer.h"

#include "base/class/ClassIdBase.h"

CREATE_ID(ElementId);
CREATE_ID(MatId);

class ElementBase : public virtual ClassIdBase<ElementId> {
public:
    ElementBase(const LayerId layId = LayerId(0),
                const MatId   matId = MatId(0));
    ElementBase(const ElementBase& rhs);
    virtual ~ElementBase();

    virtual bool operator==(const ElementBase& rhs) const;
    virtual bool operator!=(const ElementBase& rhs) const;

    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }

    virtual UInt numberOfFaces      () const = 0;
    virtual UInt numberOfVertices   () const = 0;
    virtual UInt numberOfCoordinates() const = 0;

    virtual UInt numberOfSideVertices   (const UInt f = 0) const = 0;
    virtual UInt numberOfSideCoordinates(const UInt f = 0) const = 0;

    LayerId getLayerId() const { return layId_; }
    MatId   getMatId  () const { return matId_;   }

    static vector<CoordinateId> ascendingIdOrder(
            const vector<CoordinateId>& rhs);

    template<class T>
    static vector<CoordinateId> getIds(vector<const Coordinate<T,3>*> in) {
        vector<CoordinateId> res(in.size());
        for  (UInt i = 0; i < in.size(); i++) {
            res[i] = in[i]->getId();
        }
        return res;
    }

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
    class ErrorCoord : public Error {
    public:
        ErrorCoord(const CoordinateId& coordId);
        virtual ~ErrorCoord() throw();

        CoordinateId getCoordId() const { return coordId_; }
    private:
        CoordinateId coordId_;
    };

    class ErrorCoordNotFound : public ErrorCoord {
    public:
        ErrorCoordNotFound(const CoordinateId& coordId);
        virtual ~ErrorCoordNotFound() throw();
    };

    class ErrorCoordNotCoincident : public ErrorCoord {
    public:
        ErrorCoordNotCoincident(const CoordinateId& coordId);
        virtual ~ErrorCoordNotCoincident() throw();
    };

    Element();
    virtual ~Element();

    bool operator== (const ElementBase& rhs) const;

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

    vector<const Coordinate<T,3>*> getVertices() const;
    vector<const Coordinate<T,3>*> getSideVertices(const UInt face) const;

    virtual void setV(const UInt i, const Coordinate<T,3>*);

    virtual Element<Int>* toStructured(const GroupCoordinates<CoordI3>&,
                                       const Grid3&,
                                       const Real = Grid3::tolerance) const;
    virtual Element<Real>* toUnstructured(const GroupCoordinates<CoordR3>&,
                                          const Grid3&) const;

protected:

    bool vertexInCell (const Grid3& grid, const Real tol) const;
    bool vertexInBound() const;
    CoordinateId* vertexToStructured(const GroupCoordinates<CoordI3>& cG,
                                     const Grid3& grid,
                                     const Real tol) const;
    CoordinateId* vertexToUnstructured(const GroupCoordinates<CoordR3>& cG,
                                       const Grid3& grid) const;

};

typedef ElementBase   Elem;
typedef Element<Real> ElemR;
typedef Element<Int> ElemI;

#endif
