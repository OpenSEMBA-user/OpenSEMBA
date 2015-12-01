// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#ifndef SEMBA_GEOMETRY_ELEMENT_ELEMENT_H_
#define SEMBA_GEOMETRY_ELEMENT_ELEMENT_H_

#include "math/vector/Cartesian.h"
#include "geometry/Box.h"
#include "geometry/Grid.h"
#include "geometry/coordinate/Group.h"
#include "geometry/layer/Group.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"
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

typedef Layer::Layer               Layer;
typedef Class::Identifiable<MatId> Model;

class Base;
typedef Class::Identification<Base> Id;

class Base : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Printable,
             public virtual Class::Identifiable<Id> {
public:
    Base(const Layer* lay = NULL,
         const Model* mat = NULL);
    Base(const Base& rhs);
    virtual ~Base();

    virtual bool operator==(const Base& rhs) const;
    virtual bool operator!=(const Base& rhs) const;

    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }

    virtual Size numberOfFaces      () const = 0;
    virtual Size numberOfVertices   () const = 0;
    virtual Size numberOfCoordinates() const = 0;

    virtual Size numberOfSideVertices   (const Size f = 0) const = 0;
    virtual Size numberOfSideCoordinates(const Size f = 0) const = 0;

    LayerId getLayerId() const;
    MatId   getMatId  () const;

    const Layer* getLayer() const { return lay_; }
    const Model* getModel() const { return mat_; }

    static std::vector<CoordId> ascendingIdOrder(
            const std::vector<CoordId>& rhs);

    template<class T1, class T2>
    static bool areSameCoords(
            const std::vector<const T1*>& lhs,
            const std::vector<const T2*>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        std::vector<CoordId> lhsId, rhsId;
        for (Size i = 0; i < lhs.size(); i++) {
            lhsId.push_back(lhs[i]->getId());
            rhsId.push_back(rhs[i]->getId());
        }
        return (ascendingIdOrder(lhsId) == ascendingIdOrder(rhsId));
    }

    template<class T>
    static std::vector<CoordId> getIds(
            std::vector<const Coordinate::Coordinate<T,3>*> in) {
        std::vector<CoordId> res(in.size());
        for  (Size i = 0; i < in.size(); i++) {
            res[i] = in[i]->getId();
        }
        return res;
    }

    virtual void setLayer(const Layer* lay) { lay_ = lay; }
    virtual void setModel(const Model* mat) { mat_ = mat; }

    virtual void printInfo() const = 0;

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

    virtual const Coordinate::Coordinate<T,3>* getV    (const Size i) const = 0;
    virtual const Coordinate::Coordinate<T,3>* getSideV(const Size f,
                                                        const Size i) const = 0;

    virtual const Coordinate::Coordinate<T,3>* getVertex    (
            const Size i) const = 0;
    virtual const Coordinate::Coordinate<T,3>* getSideVertex(
            const Size f,
            const Size i) const = 0;

    Box<T,3> getBound() const;
    // Returns ptr to coord with min(max) lexicographical position.
    virtual const Coordinate::Coordinate<T,3>* getMinV() const;
    virtual const Coordinate::Coordinate<T,3>* getMaxV() const;

    std::vector<const Coordinate::Coordinate<T,3>*>getVertices() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getCoordinates() const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideCoordinates(
            const Size face) const;
    std::vector<const Coordinate::Coordinate<T,3>*> getSideVertices(
            const Size face) const;

    virtual void setV(const Size i, const Coordinate::Coordinate<T,3>*);

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

#endif /* SEMBA_GEOMETRY_ELEMENT_ELEMENT_H_ */
