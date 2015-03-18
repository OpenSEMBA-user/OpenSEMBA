/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD4_H_
#define QUAD4_H_

#include "Quad.h"

template<class T = void>
class Quad4;

template<>
class Quad4<void> : public virtual Surf {
public:
    Quad4() {}
    virtual ~Quad4() {}

    UInt numberOfCoordinates() const { return 4; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 2; }

};

template<class T>
class Quad4: public virtual Quad<T>,
             public virtual Quad4<void> {
public:
    Quad4();
	Quad4(const CoordinateGroup<Coordinate<T,3> >&,
          const ElementId id,
          const CoordinateId vId[4],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
	Quad4(CoordinateGroup<Coordinate<T,3> >&,
	      const ElementId id,
	      const Box<T,3>& box,
	      const LayerId layerId = LayerId(0),
	      const MatId   matId   = MatId(0));
    Quad4(const Quad4<T>& rhs);
	virtual ~Quad4();
    
	ClassBase* clone() const;
    
	const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
	const Coordinate<T,3>* getSideV(const UInt f,
	                           const UInt i) const;

	const Coordinate<T,3>* getVertex    (const UInt i) const;
	const Coordinate<T,3>* getSideVertex(const UInt f,
	                                const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>*);
    
	void printInfo() const;
	void check() const;

private:
	const Coordinate<T,3>* v_[4];
};

typedef Quad4<Real> QuadR4;
typedef Quad4<Int > QuadI4;

#endif /* QUAD4_H_ */
