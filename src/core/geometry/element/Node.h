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

#ifndef SEMBA_GEOMETRY_ELEMENT_NODE_H_
#define SEMBA_GEOMETRY_ELEMENT_NODE_H_

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class NodeBase : public virtual Base {
public:
    NodeBase() {};
    virtual ~NodeBase() {};

    inline std::size_t numberOfCoordinates() const { return 1; }
    inline std::size_t numberOfFaces   () const { return 1; }
    inline std::size_t numberOfVertices() const { return 1; }
    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const {
        return 1;
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const {
        return 1;
    }
};

template<class T>
class Node : public virtual Element<T>,
             public virtual NodeBase {
public:
    Node();
    Node(const Id id,
         const Coordinate::Coordinate<T,3>* v[1],
         const Layer* lay = NULL,
         const Model* mat = NULL);
    Node(Coordinate::Group<Coordinate::Coordinate<T,3> >&,
         const Id id,
         const Box<T,3>& box,
         const Layer* lay = NULL,
         const Model* mat = NULL);
    Node(const Node<T>& rhs);
    virtual ~Node();
    
    SEMBA_CLASS_DEFINE_CLONE(Node<T>);

    bool isStructured(const Grid3&, const Math::Real = Grid3::tolerance) const;

    const Coordinate::Coordinate<T,3>* getV    (const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideV(const std::size_t f,
                                                const std::size_t i) const;

    const Coordinate::Coordinate<T,3>* getVertex    (
            const std::size_t i) const;
    const Coordinate::Coordinate<T,3>* getSideVertex(
            const std::size_t f,
            const std::size_t i) const;

    void setV(const std::size_t i, const Coordinate::Coordinate<T,3>* coord);

    ElemI* toStructured(const Coordinate::Group<CoordI3>&,
                        const Grid3&,
                        const Math::Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const Coordinate::Group<CoordR3>&,
                          const Grid3&) const;

    void printInfo() const;

private:
    const Coordinate::Coordinate<T,3>* v_[1];
};

} /* namespace Element */

typedef Element::NodeBase         Nod;
typedef Element::Node<Math::Real> NodR;
typedef Element::Node<Math::Int > NodI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Node.hpp"

#endif /* SEMBA_GEOMETRY_ELEMENT_NODE_H_ */
