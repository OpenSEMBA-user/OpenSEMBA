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
#ifndef NODE_H_
#define NODE_H_

#include "Element.h"

class NodeBase : public virtual ElementBase {
public:
    NodeBase() {};
    virtual ~NodeBase() {};

    inline UInt numberOfCoordinates() const { return 1; }
    inline UInt numberOfFaces   () const { return 1; }
    inline UInt numberOfVertices() const { return 1; }
    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 1; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 1; }
};

template<class T>
class Node : public virtual Element<T>,
             public virtual NodeBase {
public:
    Node();
	Node(const GroupCoordinates< Coordinate<T,3> >&,
         const ElementId id,
         const CoordinateId vId[1],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Node(const ElementId id,
         const Coordinate<T,3>* v[1],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Node(GroupCoordinates<Coordinate<T,3> >&,
         const ElementId id,
         const Box<T,3>& box,
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Node(const Node<T>& rhs);
	virtual ~Node();
    
    DEFINE_CLONE(Node<T>);

    bool isStructured(const Grid3&, const Real = Grid3::tolerance) const;

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f, const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f, const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    ElemI* toStructured(const GroupCoordinates<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const GroupCoordinates<CoordR3>&, const Grid3&) const;

	void printInfo() const;

private:
	const Coordinate<T,3>* v_[1];
};

typedef NodeBase   Nod;
typedef Node<Real> NodR;
typedef Node<Int > NodI;

#endif
