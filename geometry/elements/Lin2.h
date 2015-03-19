/*
 * Lin2.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef LIN2_H_
#define LIN2_H_

#include "Line.h"

template<class T = void>
class Lin2;

template<>
class Lin2<void> : public virtual Line<void> {
public:
    Lin2() {};
    virtual ~Lin2() {};

    inline UInt numberOfCoordinates() const { return 2; }
};

template<class T>
class Lin2 : public virtual Line<T>,
             public virtual Lin2<void> {
public:
    Lin2();
	Lin2(const CoordinateGroup<Coordinate<T,3> >&,
         const ElementId id,
         const CoordinateId vId[2],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Lin2(const ElementId id,
         const Coordinate<T,3>* v[2],
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
	Lin2(CoordinateGroup<Coordinate<T,3> >&,
         const ElementId id,
         const Box<T,3>& box,
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Lin2(const Lin2<T>& rhs);
	virtual ~Lin2();
    
    ClassBase* clone() const;

    const Coordinate<T,3>* getV    (const UInt i) const { return v_[i]; }
    const Coordinate<T,3>* getSideV(const UInt f,
                               const UInt i) const;

    const Coordinate<T,3>* getVertex    (const UInt i) const;
    const Coordinate<T,3>* getSideVertex(const UInt f,
                                         const UInt i) const;

    void setV(const UInt i, const Coordinate<T,3>* coord);

    ElemI* toStructured(CoordinateGroup<CoordI3>&, const Grid3&) const;

    void printInfo() const;
private:
    static const SimplexLin<1> lin;
    const Coordinate<T,3>* v_[2];
};

typedef Lin2<Real> LinR2;
typedef Lin2<Int > LinI2;

#endif /* LIN2_H_ */
