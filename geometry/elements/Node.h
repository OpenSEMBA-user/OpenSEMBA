#ifndef NODE_H_
#define NODE_H_

#include "Node.h"

template<class T>
class Node : public Element<T>{
public:
    Node();
	Node(const CoordinateGroup<>&,
         const ElementId id,
         const CoordinateId vId[1],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Node(const ElementId id,
         const Coordinate<T,3>* v[1],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Node(const Node<T>& rhs);
	virtual ~Node();
    
    ClassBase* clone() const;
    
    inline UInt numberOfCoordinates() const { return 1; }
    inline UInt numberOfFaces   () const { return 1; }
    inline UInt numberOfVertices() const { return 1; }
    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 1; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 1; }

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f, const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f, const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);
	
	void printInfo() const;
private:
	const Coordinate<T,3>* v_[1];
};

#include "Node.hpp"

typedef Node<Real> NodeR;
typedef Node<Int > NodeI;

#endif
