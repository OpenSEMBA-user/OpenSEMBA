/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD4_H_
#define QUAD4_H_

#include "Quad.h"

class Quad4Base : public virtual SurfaceBase {
public:
    Quad4Base() {}
    virtual ~Quad4Base() {}

    UInt numberOfCoordinates() const { return 4; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 2; }
};

template<class T>
class Quad4: public virtual Quad<T>,
             public virtual Quad4Base {
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
    
    DEFINE_CLONE(Quad4<T>);

    bool isStructured(const Grid3&, const Real = Grid3::tolerance) const;

	const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
	const Coordinate<T,3>* getSideV(const UInt f,
	                                const UInt i) const;

	const Coordinate<T,3>* getVertex    (const UInt i) const;
	const Coordinate<T,3>* getSideVertex(const UInt f,
	                                     const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>*);

    ElemI* toStructured(CoordinateGroup<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(CoordinateGroup<CoordR3>&, const Grid3&) const;

	void printInfo() const;
	void check() const;

private:
	const Coordinate<T,3>* v_[4];
};

typedef Quad4Base   Qua4;
typedef Quad4<Real> QuaR4;
typedef Quad4<Int > QuaI4;

#endif /* QUAD4_H_ */
