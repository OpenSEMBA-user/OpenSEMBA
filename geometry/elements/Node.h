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
	Node(const CoordinateGroup< Coordinate<T,3> >&,
         const ElementId id,
         const CoordinateId vId[1],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Node(const ElementId id,
         const Coordinate<T,3>* v[1],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Node(CoordinateGroup<Coordinate<T,3> >&,
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

    vector<const Coordinate<T,3>*> getVertices() const;
    vector<const Coordinate<T,3>*> getSideVertices(const UInt face) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    ElemI* toStructured(const CoordinateGroup<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const CoordinateGroup<CoordR3>&, const Grid3&) const;

	void printInfo() const;

private:
	const Coordinate<T,3>* v_[1];
};

typedef NodeBase   Nod;
typedef Node<Real> NodR;
typedef Node<Int > NodI;

#endif
