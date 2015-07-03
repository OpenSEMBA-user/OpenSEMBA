/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD4_H_
#define QUAD4_H_

#include <geometry/elements/Quadrilateral.h>

class Quadrilateral4Base : public virtual SurfaceBase {
public:
    Quadrilateral4Base() {}
    virtual ~Quadrilateral4Base() {}

    UInt numberOfCoordinates() const { return 4; }

    UInt numberOfSideCoordinates(const UInt f = 0) const { return 2; }
};

template<class T>
class Quadrilateral4: public virtual Quadrilateral<T>,
                      public virtual Quadrilateral4Base {
public:
    Quadrilateral4();
	Quadrilateral4(const GroupCoordinates<Coordinate<T,3> >&,
          const ElementId id,
          const CoordinateId vId[4],
          const LayerId layerId = LayerId(0),
          const MatId   matId   = MatId(0));
	Quadrilateral4(GroupCoordinates<Coordinate<T,3> >&,
	      const ElementId id,
	      const Box<T,3>& box,
	      const LayerId layerId = LayerId(0),
	      const MatId   matId   = MatId(0));
    Quadrilateral4(const Quadrilateral4<T>& rhs);
	virtual ~Quadrilateral4();
    
    DEFINE_CLONE(Quadrilateral4<T>);

    bool isStructured(const Grid3&, const Real = Grid3::tolerance) const;

	const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
	const Coordinate<T,3>* getSideV(const UInt f,
	                                const UInt i) const;

	const Coordinate<T,3>* getVertex    (const UInt i) const;
	const Coordinate<T,3>* getSideVertex(const UInt f,
	                                     const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>*);

    ElemI* toStructured(const GroupCoordinates<CoordI3>&,
                        const Grid3&, const Real = Grid3::tolerance) const;
    ElemR* toUnstructured(const GroupCoordinates<CoordR3>&, const Grid3&) const;

	void printInfo() const;
	void check() const;

private:
	const Coordinate<T,3>* v_[4];
};

typedef Quadrilateral4Base   Qua4;
typedef Quadrilateral4<Real> QuaR4;
typedef Quadrilateral4<Int > QuaI4;

#endif /* QUAD4_H_ */
