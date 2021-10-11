#pragma once

#include "math/vector/Cartesian.h"
#include "geometry/Box.h"
#include "geometry/Grid.h"
#include "geometry/coordinate/Group.h"
#include "geometry/layer/Group.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Identifiable.h"
#include "class/Identification.h"

namespace SEMBA {

namespace PhysicalModel {

class PhysicalModel;
typedef Class::Identification<PhysicalModel> Id;

} /* namespace PhysicalModel */

typedef PhysicalModel::Id MatId;

namespace Geometry {
namespace Element {

typedef Geometry::Layer::Layer     Layer;
typedef Class::Identifiable<MatId> Model;

class Base;
typedef Class::Identification<Base> Id;

class Base : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Identifiable<Id> {
public:
    Base(const Layer* lay = nullptr,
         const Model* mat = nullptr);
    Base(const Base& rhs);
    virtual ~Base();

    virtual bool operator==(const Base& rhs) const;
    virtual bool operator!=(const Base& rhs) const;

    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }

    virtual std::size_t numberOfFaces      () const = 0;
    virtual std::size_t numberOfVertices   () const = 0;
    virtual std::size_t numberOfCoordinates() const = 0;

    virtual std::size_t numberOfSideVertices(const std::size_t f = 0) const = 0;
    virtual std::size_t numberOfSideCoordinates(const std::size_t f = 0) const = 0;

    LayerId getLayerId() const;
    MatId   getMatId  () const;

    const Layer* getLayer() const { return lay_; }
    const Model* getModel() const { return mat_; }

    static std::vector<CoordId> ascendingIdOrder(const std::vector<CoordId>& rhs);

    template<class T1, class T2>
    static bool areSameCoords(
            const std::vector<const T1*>& lhs,
            const std::vector<const T2*>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        std::vector<CoordId> lhsId, rhsId;
        for (std::size_t i = 0; i < lhs.size(); i++) {
            lhsId.push_back(lhs[i]->getId());
            rhsId.push_back(rhs[i]->getId());
        }
        return (ascendingIdOrder(lhsId) == ascendingIdOrder(rhsId));
    }

    template<class T>
    static std::vector<CoordId> getIds(
            std::vector<const Coordinate::Coordinate<T,3>*> in) {
        std::vector<CoordId> res(in.size());
        for  (std::size_t i = 0; i < in.size(); i++) {
            res[i] = in[i]->getId();
        }
        return res;
    }

    virtual void setLayer(const Layer* lay) { lay_ = lay; }
    virtual void setModel(const Model* mat) { mat_ = mat; }

private:
    const Layer* lay_;
    const Model* mat_;
};

template<class T>
class Element : public virtual Base {
public:
    Element();
    virtual ~Element();

    bool operator== (const Base& rhs) const;

    bool isCoordinate(const Coordinate::Coordinate<T,3>* coord) const;

    virtual bool isStructured(const Grid3&,
                              const Math::Real = Grid3::tolerance) const;
    virtual bool isInnerPoint(const Math::Vector::Cartesian<T,3>& pos) const;

    virtual const Coordinate::Coordinate<T,3>* getV    (
            const std::size_t i) const = 0;
    virtual const Coordinate::Coordinate<T,3>* getSideV(
            const std::size_t f,
            const std::size_t i) const = 0;

    virtual const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const = 0;
    virtual const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const = 0;

    Box<T,3> getBound() const;
    // Returns ptr to coord with min(max) lexicographical position.
    virtual const Coordinate::Coordinate<T,3>* getMinV() const;
    virtual const Coordinate::Coordinate<T,3>* getMaxV() const;

    std::vector<const Coordinate::Coordinate<T,3>*>getVertices() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getCoordinates() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideCoordinates(
            const std::size_t face) const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideVertices(
            const std::size_t face) const;

    virtual void setV(const std::size_t i, const Coordinate::Coordinate<T,3>*);

    virtual Element<Math::Int >* toStructured(
            const Coordinate::Group<CoordI3>&,
            const Grid3&,
            const Math::Real = Grid3::tolerance) const;
    virtual Element<Math::Real>* toUnstructured(
            const Coordinate::Group<CoordR3>&,
            const Grid3&) const;

protected:

    bool vertexInCell (const Grid3& grid, const Math::Real tol) const;
    bool vertexInBound() const;
    const CoordI3** vertexToStructured(const Coordinate::Group<CoordI3>& cG,
                                       const Grid3& grid,
                                       const Math::Real tol) const;
    const CoordR3** vertexToUnstructured(const Coordinate::Group<CoordR3>& cG,
                                         const Grid3& grid) const;

};

namespace Error {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

namespace Coord {

class Error : public SEMBA::Geometry::Element::Error::Error {
public:
    Error(const CoordId& coordId) : coordId_(coordId) {}
    virtual ~Error() throw() {}

    CoordId getCoordId() const { return coordId_; }
private:
    CoordId coordId_;
};

class NotFound : public Error {
public:
    NotFound(const CoordId& coordId) : Error(coordId) {
        std::stringstream aux;
        aux << "Coordinate with Id (" << this->getCoordId() << ") not found";
        str_ = aux.str();
    }
    virtual ~NotFound() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

class NotCoincident : public Error {
public:
    NotCoincident(const CoordId& coordId) : Error(coordId) {
        std::stringstream aux;
        aux << "Coordinate with Id (" << this->getCoordId()
            << ") not coincident with previous Coordinate";
        str_ = aux.str();
    }
    virtual ~NotCoincident() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Coord */
} /* namespace Error */
} /* namespace Element */

typedef Element::Id                  ElemId;
typedef Element::Base                Elem;
typedef Element::Element<Math::Real> ElemR;
typedef Element::Element<Math::Int>  ElemI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Element.hpp"

