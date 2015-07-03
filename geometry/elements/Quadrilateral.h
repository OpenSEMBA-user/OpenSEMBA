/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD_H_
#define QUAD_H_

#include "Surface.h"

class QuadrilateralBase : public virtual SurfaceBase {
public:
    QuadrilateralBase() {}
    virtual ~QuadrilateralBase() {}

    UInt numberOfFaces   () const { return 4; }
    UInt numberOfVertices() const { return 4; }

    UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
};

template<class T>
class Quadrilateral : public virtual Surface<T>,
                      public virtual QuadrilateralBase {
public:
	Quadrilateral();
    virtual ~Quadrilateral();
};

typedef QuadrilateralBase   Qua;
typedef Quadrilateral<Real> QuaR;
typedef Quadrilateral<Int > QuaI;

#endif /* QUAD4_H_ */
